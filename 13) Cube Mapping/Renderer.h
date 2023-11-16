#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Light.h"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    ~Renderer(void);
    void RenderScene() override;
    void UpdateScene(float dt) override;

protected:
    void DrawHeightmap();
    void DrawWater();
    void DrawSkybox();

    void DrawPostProcess();
    void PresentScene();

    Shader* lightShader;
    Shader* reflectShader;
    Shader* skyboxShader;
    Shader* FlowWaterShader;
    Shader* sceneShader;
    Shader* processShader;

    HeightMap* heightMap;
    Mesh* quad;

    Light* light;
    Camera* camera;

    GLuint cubeMap;
    GLuint waterTex;
    GLuint earthTex;
    GLuint earthBump;

    float waterRotate;
    float waterCycle;

    float sceneTime = 0.0f;

    GLuint bufferFBO;
    GLuint processFBO;
    GLuint bufferColourTex[2];
    GLuint bufferDepthTex;

};