#pragma once
#include "Mesh.hpp"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    virtual ~Renderer(void);

    virtual void RenderScene();

    void UpdateTextureMatrix(float RotationValue);
    void ToggleRepeating();
    void ToggleFiltering();

protected:
    MeshTriangle* triangle;
    Shader* TextureShader;
    float scale;
    float rotation;
    Vector3 position;

    GLuint texture;
    bool filtering;
    bool repeating;
};

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

    triangle = MeshTriangle::GenerateTriangle();

    texture = SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    if (!texture) { return; }


    TextureShader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
    if (!TextureShader->LoadSuccess()) { return; }

    filtering = true;
    repeating = false;
    init = true;

}

void Renderer::RenderScene() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    BindShader(TextureShader);
    UpdateShaderMatrices();

    glUniform1i(glGetUniformLocation(TextureShader->GetProgram(), "diffuseTex"), 0); // this last parameter
    glActiveTexture(GL_TEXTURE0); // should match this number !
    glBindTexture(GL_TEXTURE_2D, texture);

    triangle->Draw();

    // set windowSize as uniform
    glUniform2f(glGetUniformLocation(TextureShader->GetProgram(), "screen"), width, height);
}

inline void Renderer::UpdateTextureMatrix(float RotationValue)
{
    Matrix4 push = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
    Matrix4 pop = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
    Matrix4 rotation = Matrix4::Rotation(RotationValue, Vector3(0, 0, 1));
    textureMatrix = pop * rotation * push;
}

inline void Renderer::ToggleRepeating()
{
    repeating = !repeating;
    SetTextureRepeating(texture, repeating);
}

inline void Renderer::ToggleFiltering()
{
    filtering = !filtering;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Renderer ::~Renderer(void) {
    delete triangle;
    delete TextureShader;
    glDeleteTextures(1, &texture);
}