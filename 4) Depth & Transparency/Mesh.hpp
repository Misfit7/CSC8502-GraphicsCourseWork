#pragma once
#include "../NCLGL/OGLRenderer.h"

using namespace std;

class MeshShape :public Mesh
{
public:
    static MeshShape* GenerateTriangle();
    static MeshShape* GenerateQuad();

private:

};

MeshShape* MeshShape::GenerateTriangle() {
    MeshShape* m = new MeshShape();
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
    m->textureCoords = new Vector2[m->numVertices]; //TextureFile start from 0,0 at left top
    m->textureCoords[0] = Vector2(0.5f, 0.0f);
    m->textureCoords[1] = Vector2(1.0f, 1.0f);
    m->textureCoords[2] = Vector2(0.0f, 1.0f);

    /*m->textureCoords[0] = Vector2(0.5f, 1.0f);
    m->textureCoords[1] = Vector2(0.0f, 0.0f);
    m->textureCoords[2] = Vector2(1.0f, 0.0f);*/

    m->BufferData();
    return m;
}

MeshShape* MeshShape::GenerateQuad()
{
    MeshShape* m = new MeshShape();
    m->numVertices = 4;
    m->type = GL_TRIANGLE_STRIP;

    m->vertices = new Vector3[m->numVertices];
    m->textureCoords = new Vector2[m->numVertices];
    m->colours = new Vector4[m->numVertices];

    m->vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
    m->vertices[1] = Vector3(-1.0f, -1.0f, 0.0f);
    m->vertices[2] = Vector3(1.0f, 1.0f, 0.0f);
    m->vertices[3] = Vector3(1.0f, -1.0f, 0.0f);

    m->textureCoords[0] = Vector2(0.0f, 1.0f);
    m->textureCoords[1] = Vector2(0.0f, 0.0f);
    m->textureCoords[2] = Vector2(1.0f, 1.0f);
    m->textureCoords[3] = Vector2(1.0f, 0.0f);

    for (int i = 0; i < 4; ++i) {
        m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    m->BufferData();
    return m;
}
