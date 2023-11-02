#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/HeightMap.h"

class Renderer : public OGLRenderer {
public:
    Renderer(Window& parent);
    ~Renderer(void);

    void RenderScene() override;
    void UpdateScene(float dt) override;

protected:
    void FillBuffers(); // G - Buffer Fill Render Pass
    void DrawPointLights(); // Lighting Render Pass
    void CombineBuffers(); // Combination Render Pass
    // Make a new texture ...
    void GenerateScreenTexture(GLuint& into, bool depth = false);

    Shader* sceneShader; // Shader to fill our GBuffers
    Shader* pointlightShader; // Shader to calculate lighting
    Shader* combineShader; // shader to stick it all together

    GLuint bufferFBO; // FBO for our G - Buffer pass
    GLuint bufferColourTex; // Albedo goes here
    GLuint bufferNormalTex; // Normals go here
    GLuint bufferDepthTex; // Depth goes here

    GLuint pointLightFBO; // FBO for our lighting pass
    GLuint lightDiffuseTex; // Store diffuse lighting
    GLuint lightSpecularTex; // Store specular lighting

    HeightMap* heightMap; // Terrain !
    Light* pointLights; // Array of lighting data
    Mesh* sphere; // Light volume
    Mesh* quad; // To draw a full - screen quad
    Camera* camera; // Our usual camera
    GLuint earthTex;
    GLuint earthBump;
};