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

    void AutoScene(float dt);

protected:
    //draw
    void DrawSkybox();
    void DrawMainScene();
    void DrawunLight(SceneNode* unLight);

    void SplitRenderScene();

    Mesh* quad;
    GLuint cubeMap;
    GLuint texture;
    GLuint Shiptexture;

    //light and skybox
    Shader* lightShader;
    Shader* skyboxShader;
    Shader* sceneShader;
    Shader* shadowShader;

    //spaceship
    SceneNode* spaceship;

    //solar system eight star
    SceneNode* root;
    SolarSystem* solar;
    SceneNode* sun;
    SceneNode* soilring;

    //build sort draw nodelist
    vector <SceneNode*> transparentNodeList;
    vector <SceneNode*> nodeList;
    void BuildNodeLists(SceneNode* from);
    void SortNodeLists();
    void ClearNodeLists();
    void DrawNodes();
    void DrawNode(SceneNode* n);

    //switchView
    int viewFlag = 1;
    int dis = 8;
    bool free = 0;

    //camera
    Light* light;
    Camera* camera;
    Camera* camera1;
    Frustum frameFrustum;
    bool splitView = 0;

    //autoplay
    float playTime = 0.0f;
    bool autoPlay = false;
    bool scene1 = false;
    bool scene2 = false;
    bool scene3 = false;
    bool scene4 = false;
    float ssAng = 0.0f;
    bool camfree = true;

};