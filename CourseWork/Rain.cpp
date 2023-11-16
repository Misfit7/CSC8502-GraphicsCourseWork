#include "Rain.h"

Rain::Rain(Vector3 p) {
    pos = p;
    Rmesh = Mesh::LoadFromMeshFile("Sphere.msh");
    Rtexture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/rain.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    for (int i = 0; i < numParticles; i++) {
        Particle particle;
        particle.position = Vector3(rand() % 1500 - 750, 200, rand() % 1500 - 750);
        float speed = (rand() % 20) + 5;
        particle.speed = Vector3(0, -speed, 0);
        particle_list.push_back(particle);
        SceneNode* rain_drop = new SceneNode();
        rain_drop->SetMesh(this->Rmesh);
        rain_drop->SetTexture(this->Rtexture);
        rain_drop->SetTransform(Matrix4::Translation(particle.position));
        rain_drop->SetModelScale(Vector3(5, 5, 5));
        rain_drop->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        rain_drop->SetBoundingRadius(5.0f);
        AddChild(rain_drop);
    }
}

void Rain::Update(float dt) {
    for (int i = 0; i < numParticles; ++i) {
        if (children[i]->GetWorldTransform().GetPositionVector().y <= 75) {
            children[i]->SetTransform(Matrix4::Translation(Vector3(rand() % 1500 - 750, 200, rand() % 1500 - 750)));
            float speed = rand() % 20 + 5;
            particle_list[i].speed = Vector3(0, -speed, 0);
        }
        children[i]->SetTransform(children[i]->GetTransform() * Matrix4::Translation(particle_list[i].speed));
    }
    SceneNode::Update(dt);
}