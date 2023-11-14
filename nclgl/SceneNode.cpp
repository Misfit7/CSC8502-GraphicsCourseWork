#include "SceneNode.h"

SceneNode::SceneNode(Mesh* m, Vector4 colour) {
    this->mesh = m;
    this->colour = colour;
}

SceneNode::~SceneNode(void) {
    for (unsigned int i = 0; i < children.size(); ++i) {
        delete children[i];
    }
}

void SceneNode::AddChild(SceneNode* s) {
    children.push_back(s);
    s->parent = this;
}

void SceneNode::Update(float dt) {
    if (parent) { //This node has a parent ...
        worldTransform = parent->worldTransform * transform;
    }
    else { //Root node , world transform is local transform!
        worldTransform = transform;
    }
    for (vector <SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
        (*i)->Update(dt);
    }
}

void SceneNode::Draw(const OGLRenderer& r) {
    if (mesh) { mesh->Draw(); }
}

void SceneNode::DrawSubmesh(const OGLRenderer& r) {
    if (mesh) {
        for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, matTextures[i]);
            mesh->DrawSubMesh(i);
        }
    }
}