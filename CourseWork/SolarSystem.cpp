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
    sun->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
    sun->SetMesh(m);
    sun->SetTexture(texture);
    sun->SetBoundingRadius(696.0f);
    AddChild(sun);

    //mercury
    mercuryR = new SceneNode();
    mercury = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/1.mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    mercury->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    mercury->SetTransform(Matrix4::Translation(Vector3(1400.0f, 0.0f, 0.0f)));
    mercury->SetModelScale(Vector3(24.4f, 24.4f, 24.4f));
    mercury->SetMesh(m);
    mercury->SetTexture(texture);
    mercury->SetBoundingRadius(24.4f);
    AddChild(mercuryR);
    mercuryR->AddChild(mercury);

    //gold
    goldR = new SceneNode();
    gold = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/2.gold.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    gold->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    gold->SetTransform(Matrix4::Translation(Vector3(1800.0f, 0.0f, 0.0f)));
    gold->SetModelScale(Vector3(60.5f, 60.5f, 60.5f));
    gold->SetMesh(m);
    gold->SetTexture(texture);
    gold->SetBoundingRadius(60.5f);
    AddChild(goldR);
    goldR->AddChild(gold);

    //earth
    earthR = new SceneNode();
    earth = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/3.earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    earth->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    earth->SetTransform(Matrix4::Translation(Vector3(2200.0f, 0.0f, 0.0f)));
    earth->SetModelScale(Vector3(63.0f, 63.0f, 63.0f));
    earth->SetMesh(m);
    earth->SetTexture(texture);
    earth->SetBoundingRadius(63.0f);
    AddChild(earthR);
    earthR->AddChild(earth);
    earthCloud = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/3.earth_clouds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    earthCloud->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.5f));
    earthCloud->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
    earthCloud->SetModelScale(Vector3(63.1f, 63.1f, 63.1f));
    earthCloud->SetMesh(m);
    earthCloud->SetTexture(texture);
    earthCloud->SetBoundingRadius(63.1f);
    earth->AddChild(earthCloud);
    //moon
    moon = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/3.5.moon.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    moon->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    moon->SetTransform(Matrix4::Translation(Vector3(136.0f, 0.0f, 0.0f)));
    moon->SetModelScale(Vector3(18.0f, 18.0f, 18.0f));
    moon->SetMesh(m);
    moon->SetTexture(texture);
    moon->SetBoundingRadius(18.0f);
    earth->AddChild(moon);

    //fire
    fireR = new SceneNode();
    fire = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/4.fire.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    fire->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    fire->SetTransform(Matrix4::Translation(Vector3(2600.0f, 0.0f, 0.0f)));
    fire->SetModelScale(Vector3(34.0f, 34.0f, 34.0f));
    fire->SetMesh(m);
    fire->SetTexture(texture);
    fire->SetBoundingRadius(34.0f);
    AddChild(fireR);
    fireR->AddChild(fire);

    //wood
    woodR = new SceneNode();
    wood = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/5.wood.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    wood->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    wood->SetTransform(Matrix4::Translation(Vector3(3400.0f, 0.0f, 0.0f)));
    wood->SetModelScale(Vector3(314.0f, 314.0f, 314.0f));
    wood->SetMesh(m);
    wood->SetTexture(texture);
    wood->SetBoundingRadius(314.0f);
    AddChild(woodR);
    woodR->AddChild(wood);

    //soil
    soilR = new SceneNode();
    soil = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/6.soil.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    soil->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    soil->SetTransform(Matrix4::Translation(Vector3(4200.0f, 0.0f, 0.0f)));
    soil->SetModelScale(Vector3(290.0f, 290.0f, 290.0f));
    soil->SetMesh(m);
    soil->SetTexture(texture);
    soil->SetBoundingRadius(290.0f);
    AddChild(soilR);
    soilR->AddChild(soil);
    //soilring
    soilring = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/6.soilring.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    soilring->SetColour(Vector4(1.0f, 1.0f, 1.0f, 0.88f));
    soilring->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
    soilring->SetModelScale(Vector3(500.0f, 500.0f, 500.0f));
    OBJMesh* objSphere = new OBJMesh("Starfield/Ring.obj");
    soilring->SetMesh(objSphere);
    soilring->SetTexture(texture);
    soilring->SetBoundingRadius(500.0f);
    soil->AddChild(soilring);

    //tianwang
    tianwangR = new SceneNode();
    tianwang = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/7.tianwang.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    tianwang->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    tianwang->SetTransform(Matrix4::Translation(Vector3(4800.0f, 0.0f, 0.0f)));
    tianwang->SetModelScale(Vector3(155.0f, 155.0f, 155.0f));
    tianwang->SetMesh(m);
    tianwang->SetTexture(texture);
    tianwang->SetBoundingRadius(155.0f);
    AddChild(tianwangR);
    tianwangR->AddChild(tianwang);

    //haiwang
    haiwangR = new SceneNode();
    haiwang = new SceneNode();
    texture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/8.haiwang.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
    haiwang->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    haiwang->SetTransform(Matrix4::Translation(Vector3(5300.0f, 0.0f, 0.0f)));
    haiwang->SetModelScale(Vector3(145.0f, 145.0f, 145.0f));
    haiwang->SetMesh(m);
    haiwang->SetTexture(texture);
    haiwang->SetBoundingRadius(145.0f);
    AddChild(haiwangR);
    haiwangR->AddChild(haiwang);
}

void SolarSystem::Update(float dt)
{
    //common rotation
    mercuryR->SetTransform(mercuryR->GetTransform() * Matrix4::Rotation(248.862f * dt, Vector3(0, 1, 0))); //1
    goldR->SetTransform(goldR->GetTransform() * Matrix4::Rotation(97.332f * dt, Vector3(0, 1, 0))); //2
    earthR->SetTransform(earthR->GetTransform() * Matrix4::Rotation(60.0f * dt, Vector3(0, 1, 0))); //3
    fireR->SetTransform(fireR->GetTransform() * Matrix4::Rotation(31.878f * dt, Vector3(0, 1, 0))); //4
    woodR->SetTransform(woodR->GetTransform() * Matrix4::Rotation(5.058f * dt, Vector3(0, 1, 0))); //5
    soilR->SetTransform(soilR->GetTransform() * Matrix4::Rotation(2.04f * dt, Vector3(0, 1, 0))); //6
    tianwangR->SetTransform(tianwangR->GetTransform() * Matrix4::Rotation(0.714f * dt, Vector3(0, 1, 0))); //7
    haiwangR->SetTransform(haiwangR->GetTransform() * Matrix4::Rotation(0.366f * dt, Vector3(0, 1, 0))); //8

    //planet self rotation
    sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(-2.4f * dt, Vector3(0, 1, 0))); //0
    mercury->SetTransform(mercury->GetTransform() * Matrix4::Rotation(-1.02f * dt, Vector3(0, 1, 0))); //1
    gold->SetTransform(gold->GetTransform() * Matrix4::Rotation(-0.25f * dt, Vector3(0, 1, 0))); //2
    earth->SetTransform(earth->GetTransform() * Matrix4::Rotation(-120.0f * dt, Vector3(0, 1, 0))); //3
    moon->SetTransform(moon->GetTransform() * Matrix4::Rotation(-30.0f * dt, Vector3(0, 1, 0))); //3.5
    fire->SetTransform(fire->GetTransform() * Matrix4::Rotation(-60.0f * dt, Vector3(0, 1, 0))); //4
    wood->SetTransform(wood->GetTransform() * Matrix4::Rotation(-144.0f * dt, Vector3(0, 1, 0))); //5
    soil->SetTransform(soil->GetTransform() * Matrix4::Rotation(-138.0f * dt, Vector3(0, 1, 0))); //6
    soilring->SetTransform(soilring->GetTransform() * Matrix4::Rotation(138.0f * dt, Vector3(0, 1, 0))); //6
    tianwang->SetTransform(tianwang->GetTransform() * Matrix4::Rotation(-90.0f * dt, Vector3(0, 1, 0))); //7
    haiwang->SetTransform(haiwang->GetTransform() * Matrix4::Rotation(-84.0f * dt, Vector3(0, 1, 0))); //8

    SceneNode::Update(dt);

}
