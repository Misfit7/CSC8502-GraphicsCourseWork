#include "Renderer.h"

const int POST_PASSES = 10;

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
    quad = Mesh::GenerateQuad();

    heightMap = new HeightMap(TEXTUREDIR "noise.png");

    waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    earthBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg",
        TEXTUREDIR"rusted_up.jpg", TEXTUREDIR"rusted_down.jpg",
        TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    if (!earthTex || !earthBump || !cubeMap || !waterTex) {
        return;
    }

    SetTextureRepeating(earthTex, true);
    SetTextureRepeating(earthBump, true);
    SetTextureRepeating(waterTex, true);

    reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
    skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
    lightShader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
    sceneShader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
    processShader = new Shader("TexturedVertex.glsl", "processfrag.glsl");

    if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() || !lightShader->LoadSuccess()) {
        return;
    }

    Vector3 heightmapSize = heightMap->GetHeightmapSize();

    camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 2.5f, 0.5f));
    light = new Light(heightmapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 1, 1, 1), heightmapSize.x);

    projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);


    // Generate our scene depth texture ...
    glGenTextures(1, &bufferDepthTex);
    glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    // And our colour texture ...
    for (int i = 0; i < 2; ++i) {
        glGenTextures(1, &bufferColourTex[i]);
        glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    glGenFramebuffers(1, &bufferFBO); // We'll render the scene into this
    glGenFramebuffers(1, &processFBO); // And do post processing in this

    glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
    // We can check FBO attachment success using this command !
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    waterRotate = 0.0f;
    waterCycle = 0.0f;
    init = true;
}

Renderer ::~Renderer(void) {
    delete camera;
    delete heightMap;
    delete quad;
    delete reflectShader;
    delete skyboxShader;
    delete lightShader;
    delete light;
}

void Renderer::UpdateScene(float dt) {
    camera->UpdateCamera(10 * dt);
    waterRotate += dt * 2.0f; // 2 degrees a second
    waterCycle += dt * 0.25f; // 10 units a second
    sceneTime += dt;
}

void Renderer::RenderScene() {
    viewMatrix = camera->BuildViewMatrix();

    glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    DrawSkybox();
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    DrawHeightmap();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    DrawPostProcess();
    PresentScene();

}

void Renderer::DrawPostProcess() {
    glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    BindShader(processShader);
    modelMatrix.ToIdentity();
    viewMatrix.ToIdentity();
    projMatrix.ToIdentity();
    textureMatrix.ToIdentity();
    UpdateShaderMatrices();

    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(processShader->GetProgram(), "sceneTex"), 0);
    for (int i = 0; i < POST_PASSES; ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
        glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 0);
        glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
        quad->Draw();
        // Now to swap the colour buffers , and do the second blur pass
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
        glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 1);
        glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
        quad->Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    BindShader(sceneShader);
    modelMatrix.ToIdentity();
    viewMatrix.ToIdentity();
    projMatrix.ToIdentity();
    textureMatrix.ToIdentity();
    UpdateShaderMatrices();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
    glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
    quad->Draw();
}

void Renderer::DrawSkybox() {
    glDepthMask(GL_FALSE);

    BindShader(skyboxShader);
    UpdateShaderMatrices();

    quad->Draw();

    glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap() {
    BindShader(lightShader);
    SetShaderLight(*light);
    glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTex);

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earthBump);

    modelMatrix.ToIdentity(); // New !
    textureMatrix.ToIdentity();//New!

    UpdateShaderMatrices();

    heightMap->Draw();
}

void Renderer::DrawWater() {
    BindShader(reflectShader);

    glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterTex);

    glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "time"), sceneTime);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "speed"), 1.25);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "amount"), 0.01);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "height"), 0.08);

    Vector3 hSize = heightMap->GetHeightmapSize();

    modelMatrix = Matrix4::Translation(hSize * 0.5f) * Matrix4::Scale(hSize * 0.5f) * Matrix4::Rotation(90, Vector3(1, 0, 0));

    textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

    UpdateShaderMatrices();
    quad->Draw();
}