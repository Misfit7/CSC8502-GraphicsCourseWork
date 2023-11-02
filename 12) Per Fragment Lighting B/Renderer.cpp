#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
    heightMap = new HeightMap(TEXTUREDIR"noise.png");
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    bumpmap = SOIL_load_OGL_texture(TEXTUREDIR "Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    //shader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
    shader = new Shader("BumpVertex.glsl", "BumpFragment.glsl");

    if (!shader->LoadSuccess() || !texture) {
        return;
    }

    SetTextureRepeating(texture, true);
    SetTextureRepeating(bumpmap, true);
    Vector3 heightmapSize = heightMap->GetHeightmapSize();
    camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 5.0f, 0.5f));
    light = new Light(heightmapSize * Vector3(0.5f, 1.5f, 0.5f), Vector4(1, 1, 1, 1), heightmapSize.x * 0.5f);

    projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

    glEnable(GL_DEPTH_TEST);
    init = true;
}

Renderer ::~Renderer(void) {
    delete camera;
    delete heightMap;
    delete shader;
    delete light; // This bit is new ...
}

void Renderer::UpdateScene(float dt) {
    camera->UpdateCamera(dt);
    viewMatrix = camera->BuildViewMatrix();
}

void Renderer::RenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    BindShader(shader);

    glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(glGetUniformLocation(shader->GetProgram(), "bumpTex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bumpmap);

    glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    UpdateShaderMatrices();
    SetShaderLight(*light);

    heightMap->Draw();
}