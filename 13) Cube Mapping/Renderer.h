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

    Shader* lightShader;
    Shader* reflectShader;
    Shader* skyboxShader;

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
};