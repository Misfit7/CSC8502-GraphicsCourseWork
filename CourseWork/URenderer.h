#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Light.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"

#include "SolarSystem.h"

class URenderer : public OGLRenderer {
public:
    URenderer(Window& parent);
    ~URenderer(void);
    void RenderScene() override;
    void UpdateScene(float dt) override;

    void AutoScene();

protected:
    //draw
    void DrawSkybox();
    void DrawMainScene();
    void DrawShadowScene();

    Mesh* quad;
    GLuint cubeMap;
    GLuint texture;

    //light and skybox
    Shader* lightShader;
    Shader* skyboxShader;
    Shader* sceneShader;
    Shader* shadowShader;

    //solar system eight star
    SceneNode* root;
    SolarSystem* solar;

    //build sort draw nodelist
    vector <SceneNode*> transparentNodeList;
    vector <SceneNode*> nodeList;
    void BuildNodeLists(SceneNode* from);
    void SortNodeLists();
    void ClearNodeLists();
    void DrawNodes();
    void DrawNode(SceneNode* n);

    //camera
    Light* light;
    Camera* camera;
    Frustum frameFrustum;

};