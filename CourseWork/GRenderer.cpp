#include "GRenderer.h"

#include <algorithm>
#include <cstdlib>

using namespace std;

GRenderer::GRenderer(Window& parent) : OGLRenderer(parent) {
    quad = Mesh::GenerateQuad();

    heightMap = new HeightMap(TEXTUREDIR"Starfield/noise.png");

    waterTex = SOIL_load_OGL_texture(TEXTUREDIR"water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    earthTex = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    earthBump = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/grassbump.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"Starfield/day_w.jpg", TEXTUREDIR"Starfield/day_e.jpg",
        TEXTUREDIR"Starfield/day_u.jpg", TEXTUREDIR"Starfield/day_d.jpg",
        TEXTUREDIR"Starfield/day_s.jpg", TEXTUREDIR"Starfield/day_n.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    if (!earthTex || !earthBump || !cubeMap || !waterTex) {
        return;
    }

    SetTextureRepeating(earthTex, true);
    SetTextureRepeating(earthBump, true);
    SetTextureRepeating(waterTex, true);

    reflectShader = new Shader("reflectVertex.glsl", "reflectFragment.glsl");
    skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
    lightShader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
    sceneShader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");
    textureShader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
    shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");
    skinShader = new Shader("SkinningVertex.glsl", "TexturedFragment.glsl");

    if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() || !lightShader->LoadSuccess()) {
        return;
    }

    Vector3 heightmapSize = heightMap->GetHeightmapSize();

    camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 2.5f, 0.5f));
    //change light position in UpdateScene() to mimic sun rise and down;
    light = new Light(heightmapSize * Vector3(0.5f, 2.5f, 0.5f), Vector4(1, 1, 1, 1), max(heightmapSize.x, heightmapSize.z) * 10);

    root = new SceneNode();
    root->SetTransform(Matrix4::Translation(camera->GetPosition()));
    root1 = new SceneNode();
    root1->SetTransform(Matrix4::Translation(camera->GetPosition()));
    root2 = new SceneNode();
    root2->SetTransform(Matrix4::Translation(camera->GetPosition()));
    //spaceship
    spaceship = new SceneNode();
    spaceship->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    spaceship->SetTransform(Matrix4::Translation(Vector3(600.0f, -325.0f, -500.0f)));
    spaceship->SetModelScale(Vector3(25.0f, 25.0f, 25.0f));
    spaceship->SetBoundingRadius(25.0f);
    OBJMesh* objSphere = new OBJMesh("Starfield/spaceship.obj");
    spaceship->SetMesh(objSphere);
    shipTexture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/spaceshipT.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    spaceship->SetTexture(shipTexture);
    spaceship->SetBump(SOIL_load_OGL_texture(TEXTUREDIR"Starfield/spaceshipbump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
    root->AddChild(spaceship);
    //tree
    srand(unsigned(time(0)));
    for (int i = 0; i < 25; i++)
    {
        tree = new SceneNode();
        tree->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        tree->SetTransform(Matrix4::Translation(Vector3(8216 + (rand() % 1000 - 500), -525.0f, 8216 + (rand() % 1000 - 500))));
        tree->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
        tree->SetBoundingRadius(10.0f);
        mesh = Mesh::LoadFromMeshFile("Starfield/Tree.msh");
        tree->SetMesh(mesh);
        tree->SetSubMesh(1);
        material = new MeshMaterial("Starfield/Tree.mat");
        for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
            const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
            const string* filename = nullptr;
            matEntry->GetEntry("Diffuse", &filename);
            string path = TEXTUREDIR + *filename;
            GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
            tree->SetTextures(texID);
        }
        root->AddChild(tree);
    }
    //plane
    plane = new SceneNode();
    plane->SetColour(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
    plane->SetTransform(Matrix4::Translation(Vector3(-7225.0f, -3725.0f, 2325.0f)));
    plane->SetModelScale(Vector3(80.0f, 80.0f, 80.0f));
    plane->SetBoundingRadius(80.0f);
    planemesh = Mesh::LoadFromMeshFile("Starfield/Block_02.msh");
    plane->SetMesh(planemesh);
    plane->SetSubMesh(1);
    material = new MeshMaterial("Starfield/Block_02.mat");
    for (int i = 0; i < planemesh->GetSubMeshCount(); ++i) {
        const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(i);
        const string* filename = nullptr;
        matEntry->GetEntry("Diffuse", &filename);
        string path = TEXTUREDIR + *filename;
        GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        plane->SetTextures(texID);
    }
    root->AddChild(plane);
    //building
    OBJMesh* obj = new OBJMesh("Starfield/ExportScene.unity_2.obj");
    buildingTex = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/Shiny_Glass_basecolor.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    SetTextureRepeating(buildingTex, true);
    for (int i = 0; i < 5; i++)
    {
        building = new SceneNode();
        building->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        building->SetTransform(Matrix4::Translation(Vector3(-1000.0f + i * 700, -1200.0f, -3000.0f + i * 700)));
        building->SetModelScale(Vector3(30.0f, 30.0f, 30.0f));
        building->SetBoundingRadius(30.0f);
        building->SetMesh(obj);
        building->SetTexture(buildingTex);
        building->SetBump(SOIL_load_OGL_texture(TEXTUREDIR"Starfield/Shiny_Glass_normal.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
        root->AddChild(building);
    }
    //Boss
    boss = new SceneNode();
    boss->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    boss->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
    boss->SetModelScale(Vector3(20.0f, 20.0f, 20.0f));
    boss->SetBoundingRadius(20.0f);
    bossmesh = Mesh::LoadFromMeshFile("Starfield/CrabMonster.msh");
    bossanim = new MeshAnimation("Starfield/CrabMonster.anm");
    bossmaterial = new MeshMaterial("Starfield/CrabMonster.mat");
    boss->SetMesh(bossmesh);
    boss->SetSubMesh(1);
    for (int i = 0; i < bossmesh->GetSubMeshCount(); ++i) {
        const MeshMaterialEntry* matEntry = bossmaterial->GetMaterialForLayer(i);
        const string* filename = nullptr;
        matEntry->GetEntry("Diffuse", &filename);
        string path = TEXTUREDIR + *filename;
        GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
        boss->SetTextures(texID);
    }
    root1->AddChild(boss);
    //Rain
    rain = new Rain(camera->GetPosition());
    root2->AddChild(rain);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    waterRotate = 0.0f;
    waterCycle = 0.0f;
    currentFrame = 0;
    frameTime = 0.0f;
    init = true;
}

GRenderer ::~GRenderer(void) {
    delete camera;
    delete heightMap;
    delete quad;
    delete reflectShader;
    delete skyboxShader;
    delete lightShader;
    delete light;
}

void GRenderer::BuildNodeLists(SceneNode* from) {
    if (1) {
        Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
        from->SetCameraDistance(Vector3::Dot(dir, dir));
        if (from->GetColour().w < 1.0f) {
            transparentNodeList.push_back(from);
        }
        else {
            nodeList.push_back(from);
        }
    }
    for (vector <SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i) {
        BuildNodeLists((*i));
    }
}

void GRenderer::SortNodeLists() {
    sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);
    sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void GRenderer::DrawNodes() {
    for (const auto& i : nodeList) {
        DrawNode(i);
    }
    for (const auto& i : transparentNodeList) {
        DrawNode(i);
    }
}

void GRenderer::DrawNode(SceneNode* n) {
    if (n->GetMesh()) {
        Matrix4 model = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
        UpdateShaderMatrices();
        glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, model.values);
        glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
        texture = n->GetTexture();
        glActiveTexture(GL_TEXTURE0);
        SetTextureRepeating(texture, true);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), texture);
        glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
        if (n->GetBump() != 0) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, n->GetBump());
            glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 3);
        }
        if (n->GetSubMesh())
            n->DrawSubmesh(*this);
        else if (!n->GetSubMesh())
            n->Draw(*this);
    }
}

void GRenderer::ClearNodeLists() {
    transparentNodeList.clear();
    nodeList.clear();

}

void GRenderer::AutoScene() {

}

void GRenderer::AutoSun() {
    Vector3 heightmapSize = heightMap->GetHeightmapSize();
    light->SetPosition(Vector3(heightmapSize.x * 0.5 + cos(sceneTime) * (0.5 * heightmapSize.x),
        heightmapSize.y * 2.0 + sin(sceneTime) * (0.5 * heightmapSize.z), heightmapSize.z * 0.5));
}

void GRenderer::UpdateScene(float dt) {
    camera->UpdateCamera(10 * dt);
    cout << camera->GetPosition();
    waterRotate += dt * 0.25f; // 2 degrees a second
    waterCycle += dt * 0.25f; // 10 units a second

    sceneTime += dt;
    frameTime -= dt;
    while (frameTime < 0.0f) {
        currentFrame = (currentFrame + 1) % bossanim->GetFrameCount();
        frameTime += 1.0f / bossanim->GetFrameRate();
    }

    //AutoSun();
    root->Update(dt);
    root1->Update(dt);
    root2->Update(dt);
    RenderScene();

}

void GRenderer::RenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    viewMatrix = camera->BuildViewMatrix();

    DrawSkybox();
    DrawHeightmap();
    //DrawMainScene();
    //DrawActor();
    DrawRain();
    DrawWater();

}

void GRenderer::DrawSkybox() {
    glDepthMask(GL_FALSE);

    BindShader(skyboxShader);
    UpdateShaderMatrices();

    quad->Draw();

    glDepthMask(GL_TRUE);
}

void GRenderer::DrawHeightmap() {
    BindShader(lightShader);
    SetShaderLight(*light);
    glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTex);

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earthBump);

    modelMatrix.ToIdentity(); //New!
    textureMatrix.ToIdentity(); //New!

    UpdateShaderMatrices();

    heightMap->Draw();
}

void GRenderer::DrawWater() {
    BindShader(reflectShader);

    glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterTex);

    glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "time"), sceneTime);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "speed"), 1.25);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "amount"), 0.01);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "height"), 0.1);

    Vector3 hSize = heightMap->GetHeightmapSize();
    hSize.x *= 0.5f;
    hSize.z *= 0.5f;
    hSize.y *= 0.23f;
    modelMatrix = Matrix4::Translation(hSize) * Matrix4::Scale(heightMap->GetHeightmapSize() * 0.5f) * Matrix4::Rotation(90, Vector3(1, 0, 0));

    textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

    UpdateShaderMatrices();
    quad->Draw();
}

void GRenderer::DrawMainScene() {
    BindShader(lightShader);
    SetShaderLight(*light);
    BuildNodeLists(root);
    SortNodeLists();
    UpdateShaderMatrices();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    DrawNodes();
    glDisable(GL_BLEND);

    ClearNodeLists();
}

void GRenderer::DrawActor() {
    BindShader(skinShader);
    BuildNodeLists(root1);
    SortNodeLists();
    UpdateShaderMatrices();

    vector < Matrix4 > frameMatrices;

    const Matrix4* invBindPose = bossmesh->GetInverseBindPose();
    const Matrix4* frameData = bossanim->GetJointData(currentFrame);

    for (unsigned int i = 0; i < bossmesh->GetJointCount(); ++i) {
        frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
    }
    int j = glGetUniformLocation(skinShader->GetProgram(), "joints");
    glUniformMatrix4fv(j, frameMatrices.size(), false,
        (float*)frameMatrices.data());
    glUniform3fv(glGetUniformLocation(skinShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    DrawNodes();

    ClearNodeLists();
}

void GRenderer::DrawRain() {
    BindShader(textureShader);
    //SetShaderLight(*light);
    BuildNodeLists(root2);
    SortNodeLists();
    UpdateShaderMatrices();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    DrawNodes();
    glDisable(GL_BLEND);
    ClearNodeLists();
}