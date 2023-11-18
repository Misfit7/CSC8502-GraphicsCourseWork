#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/MeshMaterial.h"
#include <string>

using namespace std;

class Tree : public SceneNode
{
public:
    Tree();
    ~Tree(void) {};
    void Update(float dt) override;

protected:
    Mesh* mesh;
    GLuint texture;
    GLuint Bump;
    MeshMaterial* material;

    float d = 0;
    float x = 0.1;

};
