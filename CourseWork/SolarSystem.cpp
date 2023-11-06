#include "SolarSystem.h"
#include "../nclgl/OGLRenderer.h"

SolarSystem::SolarSystem()
{
    m = Mesh::LoadFromMeshFile("Sphere.msh");
    SetMesh(m);

    //sun
    sun = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/0.sun.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    sun->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
    sun->SetModelScale(Vector3(696.0f, 696.0f, 696.0f));
    sun->SetMesh(m);
    sun->SetTexture(texture);
    sun->SetBoundingRadius(696.0f);
    AddChild(sun);

    //mercury
    mercury = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/1.mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    mercury->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mercury->SetTransform(Matrix4::Translation(Vector3(1000.0f, 0.0f, 0.0f)));
    mercury->SetModelScale(Vector3(24.4f, 24.4f, 24.4f));
    mercury->SetMesh(m);
    mercury->SetTexture(texture);
    mercury->SetBoundingRadius(24.4f);
    AddChild(mercury);

    //gold
    gold = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/2.gold.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    gold->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    gold->SetTransform(Matrix4::Translation(Vector3(1400.0f, 0.0f, 0.0f)));
    gold->SetModelScale(Vector3(60.5f, 60.5f, 60.5f));
    gold->SetMesh(m);
    gold->SetTexture(texture);
    gold->SetBoundingRadius(60.5f);
    AddChild(gold);

    ////earth
    //earth = new SceneNode();
    //texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/3.earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    //earth->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    //earth->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
    //earth->SetModelScale(Vector3(63.0f, 63.0f, 63.0f));
    //earth->SetMesh(m);
    //earth->SetTexture(texture);
    //earth->SetBoundingRadius(63.0f);
    //AddChild(earth);
    //earthCloud = new SceneNode();
    //texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/3.earth_clouds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    //earthCloud->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
    //earthCloud->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
    //earthCloud->SetModelScale(Vector3(63.1f, 63.1f, 63.1f));
    //earthCloud->SetMesh(m);
    //earthCloud->SetTexture(texture);
    //earthCloud->SetBoundingRadius(63.1f);
    //AddChild(earthCloud);


}

void SolarSystem::Update(float dt)
{
    //solar system self_rotation
    transform = transform * Matrix4::Rotation(60.0f * dt, Vector3(0, 1, 0));
    //planet rotation
    sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(-0.0f * dt, Vector3(0, 1, 0)));
    mercury->SetTransform(mercury->GetTransform() * Matrix4::Rotation(-0.0f * dt, Vector3(0, 1, 0)));
    gold->SetTransform(gold->GetTransform() * Matrix4::Rotation(-0.0f * dt, Vector3(0, 1, 0)));

    //earthCloud->SetTransform(sun->GetTransform() * Matrix4::Rotation(-40.0f * dt, Vector3(0, 1, 0)));

    SceneNode::Update(dt);

}
