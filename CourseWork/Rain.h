#pragma once
#include "../nclgl/SceneNode.h"

struct Particle {
    Vector3 speed;
    Vector3 position;
};

class Rain : public SceneNode {
public:
    Rain(Vector3 p);
    void Update(float dt) override;
    void SetPos(Vector3 p) { pos = p; };
protected:
    float numParticles = 150;
    vector<Particle> particle_list;
    Vector3 pos;

    Mesh* Rmesh;
    GLuint Rtexture;
};