#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    ~Renderer(void);

    void RenderScene() override;
    void UpdateScene(float dt) override;

protected:
    Camera* camera;
    Mesh* mesh;
    Shader* shader;
    MeshAnimation* anim;
    MeshMaterial* material;
    vector <GLuint> matTextures;

    int currentFrame;
    float frameTime;
};