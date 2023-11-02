#pragma once
# include "../nclgl/OGLRenderer.h"
#include "../nclgl/camera.h"
#include "../nclgl/HeightMap.h"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    ~Renderer(void);
    void RenderScene() override;
    void UpdateScene(float dt) override;
protected:
    HeightMap* heightMap;
    Shader* shader;
    Camera* camera;
    GLuint terrainTex;
};