#pragma once
#include "../NCLGL/OGLRenderer.h"

using namespace std;

class MeshTriangle :public Mesh
{
public:
    static MeshTriangle* GenerateTriangle();

private:

};

MeshTriangle* MeshTriangle::GenerateTriangle() {
    MeshTriangle* m = new MeshTriangle();
    m->numVertices = 3;

    //vertex position
    m->vertices = new Vector3[m->numVertices];
    m->vertices[0] = Vector3(0.0f, 0.5f, 0.0f);
    m->vertices[1] = Vector3(0.5f, -0.5f, 0.0f);
    m->vertices[2] = Vector3(-0.5f, -0.5f, 0.0f);

    //colour
    m->colours = new Vector4[m->numVertices];
    m->colours[0] = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    m->colours[1] = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
    m->colours[2] = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

    //texture
    m->textureCoords = new Vector2[m->numVertices]; //TextureFile start from 0.0 at left top
    m->textureCoords[0] = Vector2(0.5f, 0.0f);
    m->textureCoords[1] = Vector2(1.0f, 1.0f);
    m->textureCoords[2] = Vector2(0.0f, 1.0f);

    /*m->textureCoords[0] = Vector2(0.5f, 1.0f);
    m->textureCoords[1] = Vector2(0.0f, 0.0f);
    m->textureCoords[2] = Vector2(1.0f, 0.0f);*/

    m->BufferData();
    return m;
}