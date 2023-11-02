#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>

class SceneNode {
public:
    SceneNode() = default;
    SceneNode(Mesh* m, Vector4 colour);
    ~SceneNode(void);
    void SetTransform(const Matrix4& matrix) { transform = matrix; }
    const Matrix4& GetTransform() const { return transform; }
    Matrix4 GetWorldTransform() const { return worldTransform; }
    Vector4 GetColour() const { return colour; }
    void SetColour(Vector4 c) { colour = c; }
    Vector3 GetModelScale() const { return modelScale; }
    void SetModelScale(Vector3 s) { modelScale = s; }
    Mesh* GetMesh() const { return mesh; }
    void SetMesh(Mesh* m) { mesh = m; }
    void AddChild(SceneNode* s);
    virtual void Update(float dt);
    virtual void Draw(const OGLRenderer& r);
    std::vector <SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
    std::vector <SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

    float GetBoundingRadius() const { return boundingRadius; }
    void SetBoundingRadius(float f) { boundingRadius = f; }
    float GetCameraDistance() const { return distanceFromCamera; }
    void SetCameraDistance(float f) { distanceFromCamera = f; }
    void SetTexture(GLuint tex) { texture = tex; }
    GLuint GetTexture() const { return texture; }
    static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) { return (a->distanceFromCamera < b->distanceFromCamera) ? true : false; }

protected:
    SceneNode* parent = NULL;
    Mesh* mesh = NULL;
    Matrix4 worldTransform;
    Matrix4 transform;
    Vector3 modelScale = Vector3(1, 1, 1);
    Vector4 colour = Vector4(1, 1, 1, 1);
    std::vector <SceneNode*> children;

    float distanceFromCamera = 0.0f;
    float boundingRadius = 1.0f;
    GLuint texture = 0;
};