#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/HeightMap.h"
#include "../nclgl/Camera.h"

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
    Light* light; // A new thing !
    GLuint texture;
    GLuint bumpmap; // but we need this line added !
};
