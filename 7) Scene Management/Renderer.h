#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Camera.h"
#include "../nclgl/Frustum.h"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    ~Renderer(void);
    void UpdateScene(float msec) override;
    void RenderScene() override;
protected:
    void BuildNodeLists(SceneNode* from);
    void SortNodeLists();
    void ClearNodeLists();
    void DrawNodes();
    void DrawNode(SceneNode* n);
    SceneNode* root;
    Camera* camera;
    Shader* shader;

    Mesh* cube;
    Mesh* quad;

    GLuint texture;

    Frustum frameFrustum;
    vector <SceneNode*> transparentNodeList;
    vector <SceneNode*> nodeList;
};