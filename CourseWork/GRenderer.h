#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Light.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
#include "../nclgl/OBJMesh.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/MeshAnimation.h"

#include "Rain.h"

class GRenderer : public OGLRenderer {
public:
    GRenderer(Window& parent);
    ~GRenderer(void);
    void RenderScene() override;
    void UpdateScene(float dt) override;

    void AutoScene(float dt);

protected:
    void DrawHeightmap();
    void DrawWater();
    void DrawSkybox();
    void DrawMainScene();
    void DrawTree();
    void DrawActor();
    void DrawRain();
    void DrawShadowScene();
    void DrawBuilding();
    void DrawPostProcessScene();

    Shader* lightShader;
    Shader* reflectShader;
    Shader* skyboxShader;
    Shader* sceneShader;
    Shader* textureShader;
    Shader* shadowShader;
    Shader* skinShader;
    Shader* processShader;
    Shader* deferredsceneShader;
    Shader* pointlightShader;
    Shader* combineShader;

    //map
    HeightMap* heightMap;
    Mesh* quad;

    //anm
    MeshAnimation* anim;
    MeshMaterial* material;
    int currentFrame;
    float frameTime;
    //tree
    Mesh* mesh;
    SceneNode* tree;
    //Boss
    SceneNode* boss;
    Mesh* bossmesh;
    MeshAnimation* bossanim;
    MeshMaterial* bossmaterial;
    //spaceship
    SceneNode* root;
    SceneNode* root1;
    SceneNode* root2;
    SceneNode* spaceship;
    //plane
    SceneNode* plane;
    Mesh* planemesh;
    //building
    SceneNode* buildings;
    SceneNode* building;
    vector<SceneNode*> Vbuilding;

    //build sort draw nodelist
    vector <SceneNode*> transparentNodeList;
    vector <SceneNode*> nodeList;
    void BuildNodeLists(SceneNode* from);
    void SortNodeLists();
    void ClearNodeLists();
    void DrawNodes();
    void DrawNode(SceneNode* n);
    Frustum frameFrustum;

    //camera
    Light* light;
    Camera* camera;
    bool camfree = true;

    //material
    GLuint texture;
    GLuint cubeMap;
    GLuint waterTex;
    GLuint earthTex;
    GLuint earthBump;
    GLuint shipTexture;
    GLuint buildingTex;

    //update
    float sceneTime = 0.0f;
    float waterRotate;
    float waterCycle;

    //sun
    bool isAutoSun = 0;
    void AutoSun();

    //rain
    Rain* rain;

    //shadow
    vector <Mesh*> sceneMeshes;
    vector <Matrix4> sceneTransforms;
    GLuint shadowTex;
    GLuint shadowFBO;

    //postprocessing
    GLuint bufferFBO;
    GLuint processFBO;
    GLuint bufferColourTex[2];
    GLuint bufferDepthTex;
    void DrawPProcess();
    void PresentScene();

    //deferred shading
    bool bPointLightScene = false;
    void DrawPointLightScene();
    void FillBuffers();
    void DrawPointLights();
    void CombineBuffers();
    void GenerateScreenTexture(GLuint& into, bool depth = false);
    GLuint bufferFBO2; // FBO for our G - Buffer pass
    GLuint bufferColourTex2; // Albedo goes here
    GLuint bufferNormalTex2; // Normals go here
    GLuint bufferDepthTex2; // Depth goes here

    GLuint pointLightFBO; // FBO for our lighting pass
    GLuint lightDiffuseTex; // Store diffuse lighting
    GLuint lightSpecularTex; // Store specular lighting

    Light* pointLights; // Array of lighting data
    Mesh* sphere; // Light volume

    //autoplay
    float playTime = 0.0f;
    bool autoPlay = false;
    bool scene1 = false;
    bool scene2 = false;
    bool scene3 = false;
    bool scene4 = false;
};