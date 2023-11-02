#pragma once
#include "Mesh.hpp"
#include "../nclgl/Camera.h"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    virtual ~Renderer(void);

    virtual void RenderScene();
    virtual void UpdateScene(float dt);

    void SwitchToPerspective();
    void SwitchToOrthographic();

    inline void SetScale(float s) { scale = s; }
    inline void SetRotation(float r) { rotation = r; }
    inline void SetPosition(Vector3 p) { position = p; }

protected:
    MeshTriangle* triangle;
    Shader* matrixShader;
    float scale;
    float rotation;
    Vector3 position;

    Camera* camera;
};

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
    camera = new Camera();

    triangle = MeshTriangle::GenerateTriangle();

    matrixShader = new Shader("MatrixVertex.glsl", "colourFragment.glsl");

    if (!matrixShader->LoadSuccess()) {
        return;
    }

    init = true;

    SwitchToOrthographic();
}

void Renderer::RenderScene() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    BindShader(matrixShader);

    glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "projMatrix"), 1, false, projMatrix.values);

    glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "viewMatrix"), 1, false, viewMatrix.values);

    for (int i = 0; i < 3; ++i) {
        Vector3 tempPos = position;
        tempPos.z += (i * 500.0f);
        tempPos.x -= (i * 100.0f);
        tempPos.y -= (i * 100.0f);

        modelMatrix = Matrix4::Translation(tempPos) * Matrix4::Rotation(rotation, Vector3(0, 1, 0)) * Matrix4::Scale(Vector3(scale, scale, scale));

        glUniformMatrix4fv(glGetUniformLocation(matrixShader->GetProgram(), "modelMatrix"), 1, false, modelMatrix.values);
        triangle->Draw();

    }

    // set windowSize as uniform
    glUniform2f(glGetUniformLocation(matrixShader->GetProgram(), "screen"), width, height);
}

void Renderer::UpdateScene(float dt) {
    camera->UpdateCamera(dt);
    viewMatrix = camera->BuildViewMatrix();

}

inline void Renderer::SwitchToPerspective()
{
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
}

inline void Renderer::SwitchToOrthographic()
{
    projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

Renderer ::~Renderer(void) {
    delete triangle;
    delete matrixShader;
}