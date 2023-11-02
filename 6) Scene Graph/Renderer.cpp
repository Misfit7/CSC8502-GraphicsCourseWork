#include "Renderer.h"
#include "CubeRobot.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
    camera = new Camera();
    camera->SetPosition(Vector3(0, 30, 175));
    cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");
    shader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");
    if (!shader->LoadSuccess()) {
        return;
    }
    root = new SceneNode();
    root->AddChild(new CubeRobot(cube));
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init = true;
}
Renderer::~Renderer(void) {
    delete root;
    delete shader;
    delete camera;
    delete cube;
}

void Renderer::UpdateScene(float dt) {
    camera->UpdateCamera(dt);
    viewMatrix = camera->BuildViewMatrix();
    root->Update(dt);
}

void Renderer::RenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    BindShader(shader);
    UpdateShaderMatrices();

    glUniform1i(glGetUniformLocation(shader->GetProgram(), "diffuseTex"), 0);
    DrawNode(root);
}

void Renderer::DrawNode(SceneNode* n) {
    if (n->GetMesh()) {
        Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
        glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "modelMatrix"), 1, false, model.values);
        glUniform4fv(glGetUniformLocation(shader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
        glUniform1i(glGetUniformLocation(shader->GetProgram(), "useTexture"), 0); //Next tutorial ;)
        n->Draw(*this);
    }
    for (vector <SceneNode*>::const_iterator
        i = n->GetChildIteratorStart();
        i != n->GetChildIteratorEnd(); ++i) {
        DrawNode(*i);
    }
}