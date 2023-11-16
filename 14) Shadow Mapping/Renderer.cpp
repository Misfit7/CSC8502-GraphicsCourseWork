#include "Renderer.h"
#include<nclgl/OBJMesh.h>

#define SHADOWSIZE 2048

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
    camera = new Camera(-30.0f, 315.0f, Vector3(-8.0f, 5.0f, 8.0f));
    light = new Light(Vector3(-20.0f, 10.0f, -20.0f), Vector4(1, 1, 1, 1), 250.0f);
    sceneShader = new Shader("shadowscenevert.glsl", "shadowscenefrag.glsl");
    shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");

    if (!sceneShader->LoadSuccess() || !shadowShader->LoadSuccess()) {
        return;
    }

    sceneMeshes.emplace_back(Mesh::GenerateQuad());
    sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Sphere.msh"));
    sceneMeshes.emplace_back(Mesh::LoadFromMeshFile("Cylinder.msh"));

    sceneDiffuse = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    sceneBump = SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    SetTextureRepeating(sceneDiffuse, true);
    SetTextureRepeating(sceneBump, true);

    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    sceneTransforms.resize(4);
    sceneTransforms[0] = Matrix4::Rotation(90, Vector3(1, 0, 0)) * Matrix4::Scale(Vector3(10, 10, 1));
    sceneTime = 0.0f;
    init = true;
}

Renderer ::~Renderer(void) {
    glDeleteTextures(1, &shadowTex);
    glDeleteFramebuffers(1, &shadowFBO);

    for (auto& i : sceneMeshes) {
        delete i;
    }

    delete camera;
    delete sceneShader;
    delete shadowShader;
}

void Renderer::UpdateScene(float dt) {
    camera->UpdateCamera(dt);
    cout << camera->GetPosition();
    sceneTime += dt;
    light->SetPosition(Vector3(-20 + cos(sceneTime) * 2,
        10 + sin(sceneTime) * 2, -20));
    for (int i = 1; i < 3; ++i) { // skip the floor !
        Vector3 t = Vector3(-10 + (5 * i), 2.0f + 0, 0);
        sceneTransforms[i] = Matrix4::Translation(t);
    }
}

void Renderer::RenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    DrawShadowScene();
    DrawMainScene();
}

void Renderer::DrawShadowScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    BindShader(shadowShader);
    viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
    projMatrix = Matrix4::Perspective(1.0f, 15000.0f, 1, 45.0f);

    shadowMatrix = projMatrix * viewMatrix; // used later

    for (int i = 1; i < 3; ++i) {
        modelMatrix = sceneTransforms[i];
        UpdateShaderMatrices();
        sceneMeshes[i]->Draw();
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawMainScene() {
    BindShader(sceneShader);
    SetShaderLight(*light);
    viewMatrix = camera->BuildViewMatrix();
    projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUniform3fv(glGetUniformLocation(sceneShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sceneDiffuse);

    glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "bumpTex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sceneBump);

    glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "shadowTex"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowTex);

    for (int i = 0; i < 3; ++i) {
        modelMatrix = sceneTransforms[i];
        UpdateShaderMatrices();
        sceneMeshes[i]->Draw();
    }

}