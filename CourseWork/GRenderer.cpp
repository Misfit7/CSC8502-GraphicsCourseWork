#include "GRenderer.h"

#include <algorithm>
#include <cstdlib>

using namespace std;

#define SHADOWSIZE 2048
const int POST_PASSES = 10;

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
    //lightShader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
    lightShader = new Shader("shadowscenevert.glsl", "shadowscenefrag.glsl");
    textureShader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
    shadowShader = new Shader("shadowVert.glsl", "shadowFrag.glsl");
    skinShader = new Shader("SkinningVertex.glsl", "PerPixelFragment.glsl");
    sceneShader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
    processShader = new Shader("TexturedVertex.glsl", "processfrag.glsl");

    if (!reflectShader->LoadSuccess() || !skyboxShader->LoadSuccess() || !lightShader->LoadSuccess()) {
        return;
    }

    Vector3 heightmapSize = heightMap->GetHeightmapSize();

    camera = new Camera(-45.0f, 0.0f, heightmapSize * Vector3(0.5f, 2.5f, 0.5f));
    //change light position in UpdateScene();
    light = new Light(Vector3(heightmapSize.x,
        heightmapSize.y, 0), Vector4(1, 1, 1, 1), heightmapSize.z * 10);

    root = new SceneNode();
    root->SetTransform(Matrix4::Translation(camera->GetPosition()));
    root1 = new SceneNode();
    root1->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
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
    /*srand(unsigned(time(0)));
    for (int i = 0; i < 25; i++)
    {
        tree = new SceneNode();
        tree->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        tree->SetTransform(Matrix4::Translation(Vector3(8216 + (rand() % 500 - 250), -525.0f, 8216 + (rand() % 500 - 250))));
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
    }*/
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
    Mesh* obj = Mesh::LoadFromMeshFile("Cube.msh");
    buildingTex = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/Shiny_Glass_basecolor.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    SetTextureRepeating(buildingTex, true);
    for (int i = 0; i < 1; i++)
    {
        building = new SceneNode();
        building->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        Matrix4 trs = Matrix4::Translation(Vector3(7216.0f + i * 700, 750.0f, 6500 + i * 700));
        building->SetTransform(trs);
        building->SetModelScale(Vector3(500.0f, 1500.0f, 500.0f));
        building->SetBoundingRadius(1500.0f);
        building->SetMesh(obj);
        building->SetTexture(buildingTex);
        building->SetBump(SOIL_load_OGL_texture(TEXTUREDIR"Starfield/Shiny_Glass_normal.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
        Vbuilding.emplace_back(building);
        sceneTransforms.emplace_back(trs);
    }
    //Boss
    boss = new SceneNode();
    boss->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    boss->SetTransform(Matrix4::Translation(Vector3(4300.0f, 60.0f, 3535.0f)));
    boss->SetModelScale(Vector3(200.0f, 200.0f, 200.0f));
    boss->SetBoundingRadius(200.0f);
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

    //shadow
    glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_2D, shadowTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //postprocessing
    glGenTextures(1, &bufferDepthTex);
    glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    for (int i = 0; i < 2; ++i) {
        glGenTextures(1, &bufferColourTex[i]);
        glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
    glGenFramebuffers(1, &bufferFBO);
    glGenFramebuffers(1, &processFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

    glDeleteTextures(1, &shadowTex);
    glDeleteFramebuffers(1, &shadowFBO);
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
        SetTextureRepeating(texture, true);
        glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (n->GetBump() != 0) {
            glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 3);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, n->GetBump());

            glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 4);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, shadowTex);
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
    light->SetPosition(Vector3(heightmapSize.x * 0.5 + cos(sceneTime) * (heightmapSize.x * 0.5),
        2000 + sin(sceneTime) * 1000, 5600));
}

void GRenderer::UpdateScene(float dt) {
    camera->UpdateCamera(10 * dt);
    Vector3 camPos = camera->GetPosition();
    if (camPos.y <= 15) { camera->SetPosition(Vector3(camPos.x, 10, camPos.z)); };
    //cout << camPos;
    //cout << light->GetPosition();
    waterRotate += dt * 0.25f; // 2 degrees a second
    waterCycle += dt * 0.25f; // 10 units a second

    sceneTime += dt;
    frameTime -= dt;
    while (frameTime < 0.0f) {
        currentFrame = (currentFrame + 1) % bossanim->GetFrameCount();
        frameTime += 1.0f / bossanim->GetFrameRate();
    }
    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T)) {
        isAutoSun = !isAutoSun;
    }
    if (isAutoSun) { AutoSun(); }

    root->Update(dt);
    root1->Update(dt);
    root2->Update(dt);
    root2->SetTransform(Matrix4::Translation(Vector3(camPos.x + 10, camPos.y + 10, camPos.z - 10)));

    if (camPos.y <= 60) {
        DrawPostProcessScene();
        //RenderScene();
    }
    else {
        RenderScene();
        //DrawPostProcessScene();
    }

}

void GRenderer::RenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    viewMatrix = camera->BuildViewMatrix();

    DrawSkybox();
    DrawHeightmap();
    DrawMainScene();
    DrawBuilding();
    //DrawShadowScene();
    DrawActor();
    DrawRain();
    DrawWater();

}

void GRenderer::DrawPostProcessScene() {
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    viewMatrix = camera->BuildViewMatrix();
    glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    DrawSkybox();
    DrawHeightmap();
    DrawMainScene();
    DrawBuilding();
    DrawActor();
    DrawRain();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    DrawPProcess();
    PresentScene();

}

void GRenderer::DrawPProcess() {
    glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    BindShader(processShader);
    modelMatrix.ToIdentity();
    viewMatrix.ToIdentity();
    projMatrix.ToIdentity();
    UpdateShaderMatrices();

    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(processShader->GetProgram(), "sceneTex"), 0);
    for (int i = 0; i < POST_PASSES; ++i) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
        glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 0);
        glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
        quad->Draw();
        // Now to swap the colour buffers , and do the second blur pass
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
        glUniform1i(glGetUniformLocation(processShader->GetProgram(), "isVertical"), 1);
        glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
        quad->Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
}

void GRenderer::PresentScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    BindShader(sceneShader);
    modelMatrix.ToIdentity();
    viewMatrix.ToIdentity();
    projMatrix.ToIdentity();
    textureMatrix.ToIdentity();
    UpdateShaderMatrices();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
    glUniform1i(glGetUniformLocation(sceneShader->GetProgram(), "diffuseTex"), 0);
    quad->Draw();
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

    modelMatrix.ToIdentity();
    textureMatrix.ToIdentity();

    UpdateShaderMatrices();

    heightMap->Draw();
}

void GRenderer::DrawWater() {
    BindShader(reflectShader);
    glDisable(GL_CULL_FACE);

    glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterTex);

    glUniform1i(glGetUniformLocation(reflectShader->GetProgram(), "cubeTex"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "time"), sceneTime);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "speed"), 1.25);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "count"), 0.01);
    glUniform1f(glGetUniformLocation(reflectShader->GetProgram(), "height"), 0.1);

    Vector3 hSize = heightMap->GetHeightmapSize();
    hSize.x *= 0.5f;
    hSize.z *= 0.5f;
    hSize.y *= 0.23f;
    modelMatrix = Matrix4::Translation(hSize) * Matrix4::Scale(heightMap->GetHeightmapSize() * 0.5f) * Matrix4::Rotation(90, Vector3(1, 0, 0));

    textureMatrix = Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) * Matrix4::Scale(Vector3(10, 10, 10)) * Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

    UpdateShaderMatrices();
    quad->Draw();
    glDisable(GL_CULL_FACE);
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

void GRenderer::DrawShadowScene() {
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    BindShader(shadowShader);
    Vector3 hmapsize = heightMap->GetHeightmapSize();
    Vector3 ltPos = light->GetPosition();

    viewMatrix = Matrix4::BuildViewMatrix(Vector3(ltPos), Vector3(5700, 125, 8000));
    projMatrix = Matrix4::Perspective(1, 10000, 1, 45);
    shadowMatrix = projMatrix * viewMatrix; // used later

    for (int i = 0; i < 1; ++i) {
        modelMatrix = sceneTransforms[i] * Matrix4::Scale(Vector3(1, 1, 1));
        UpdateShaderMatrices();
        Vbuilding[i]->GetMesh()->Draw();
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glViewport(0, 0, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    viewMatrix = camera->BuildViewMatrix();
}

void GRenderer::DrawBuilding() {
    BindShader(lightShader);
    SetShaderLight(*light);
    viewMatrix = camera->BuildViewMatrix();
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

    glUniform3fv(glGetUniformLocation(lightShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "diffuseTex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buildingTex);

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "bumpTex"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, building->GetBump());

    glUniform1i(glGetUniformLocation(lightShader->GetProgram(), "shadowTex"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, shadowTex);

    for (int i = 0; i < 1; ++i) {
        modelMatrix = sceneTransforms[i] * Matrix4::Scale(Vbuilding[i]->GetModelScale());
        UpdateShaderMatrices();
        Vbuilding[i]->GetMesh()->Draw();
    }
}

void GRenderer::DrawActor() {
    BindShader(skinShader);
    SetShaderLight(*light);
    BuildNodeLists(root1);
    SortNodeLists();
    UpdateShaderMatrices();

    vector <Matrix4> frameMatrices;

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