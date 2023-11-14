#pragma once
#include "Mesh.hpp"

using namespace std;

class Renderer :public OGLRenderer {
public:
    Renderer(Window& parent);
    virtual ~Renderer(void);
    virtual void RenderScene();

protected:
    MeshTriangle* triangle;
    Shader* basicShader;

};

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
    triangle = MeshTriangle::GenerateTriangle();

    basicShader = new Shader("basicVertex.glsl", "colourFragment.glsl");

    if (!basicShader->LoadSuccess()) {
        return;
    }
    init = true;
}

void Renderer::RenderScene() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set windowSize as uniform
    glUniform2f(glGetUniformLocation(basicShader->GetProgram(), "screen"), width, height);

    BindShader(basicShader);
    triangle->Draw();
}

Renderer ::~Renderer(void) {
    delete triangle;
    delete basicShader;
}
