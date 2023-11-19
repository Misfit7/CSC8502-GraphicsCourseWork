#include "URenderer.h"

#include <algorithm>

URenderer::URenderer(Window& parent) :OGLRenderer(parent) {
    camera = new Camera(-10.0f, 0.0f, (Vector3(2987.32f, 2017.49f, 1487.84f)));
    camera1 = new Camera();
    light = new Light(Vector3(0.0f, 0.0f, 0.0f), Vector4(1, 1, 1, 1), 10000.0f);
    quad = Mesh::GenerateQuad();

    //shader
    skyboxShader = new Shader("skyboxVertex.glsl", "skyboxFragment.glsl");
    sceneShader = new Shader("SceneVertex.glsl", "SceneFragment.glsl");
    lightShader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");
    if (!sceneShader->LoadSuccess() || !skyboxShader->LoadSuccess()) {
        return;
    }

    //skybox
    cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"Starfield/W.png", TEXTUREDIR"Starfield/E.png",
        TEXTUREDIR"Starfield/U.png", TEXTUREDIR"Starfield/D.png",
        TEXTUREDIR"Starfield/S.png", TEXTUREDIR"Starfield/N.png", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    //planet
    root = new SceneNode();
    solar = new SolarSystem();
    root->AddChild(solar);
    sun = solar->GetSun();
    soilring = solar->GetSoilring();

    //spaceship
    spaceship = new SceneNode();
    spaceship->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    spaceship->SetTransform(Matrix4::Translation(Vector3(2987.32f, 2000.49f, 1487.84f)));
    spaceship->SetModelScale(Vector3(1.0f, 1.0f, 1.0f));
    spaceship->SetBoundingRadius(1.0f);
    OBJMesh* objSphere = new OBJMesh("Starfield/spaceship.obj");
    spaceship->SetMesh(objSphere);
    Shiptexture = SOIL_load_OGL_texture(TEXTUREDIR"Starfield/spaceshipT.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    spaceship->SetTexture(Shiptexture);
    spaceship->SetBump(SOIL_load_OGL_texture(TEXTUREDIR"Starfield/spaceshipbump.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
    root->AddChild(spaceship);

    //glFuntion
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    init = true;
}

URenderer ::~URenderer(void) {
    delete root;
    delete quad;
    delete camera;
    delete skyboxShader;
    delete sceneShader;
    glDeleteTextures(1, &texture);
}

void URenderer::AutoScene(float dt) {
    Vector3 sPosition = solar->fire->GetWorldTransform().GetPositionVector();
    Vector3 shipPosition = spaceship->GetWorldTransform().GetPositionVector();
    Vector3 camPos = camera->GetPosition();

    if (!scene1) {
        dis = 8;
        playTime += dt;
        sPosition.y += 350;
        camera->SetPosition(sPosition);
        double deltaX = sPosition.x;
        double deltaZ = sPosition.z;
        double angleRad = std::atan2(deltaZ, deltaX);
        double angleDeg = -angleRad * (180.0 / 3.1415);
        camera->SetYaw(angleDeg + 90);
        if (camera->GetYaw() <= 0) camera->SetYaw(camera->GetYaw() + 360.0f);
        else if (camera->GetYaw() >= 360) camera->SetYaw(camera->GetYaw() - 360.0f);
        if (playTime >= 2.0f && camPos.x >= 2599.5) scene1 = true;
    }
    if (scene1 && !scene2) {
        dis = 8;
        if (camPos.x >= 0)
            camPos.x -= 10;
        if (camPos.y <= 1100)
            camPos.y += 10;
        camera->SetPosition(camPos);
        if (camera->GetYaw() >= 0)
            camera->SetYaw(camera->GetYaw() - 1);
        if (camPos.x <= 0 && camPos.y >= 1100 && camera->GetYaw() <= 0)
            scene2 = true;
    }

    if (scene1 && scene2 && !scene3) {
        dis = 8;
        if (camera->GetYaw() <= 360)
            camera->SetYaw(camera->GetYaw() + 1);
        if (camera->GetYaw() >= 360) {
            scene3 = true;
            if (camera->GetYaw() >= 360) camera->SetYaw(camera->GetYaw() - 360.0f);
        }
    }

    if (scene1 && scene2 && scene3) {
        if (camPos.z <= 6000)
            camPos.z += 10;
        camera->SetPosition(camPos);
        dis = 12;
        if (camera->GetPitch() >= -22)
            camera->SetPitch(camera->GetPitch() - 1);
    }

}

void URenderer::UpdateScene(float dt) {
    if (camfree) camera->UpdateCamera(10 * dt);
    Vector3 camPos = camera->GetPosition();
    cout << camPos << " " << camera->GetPitch() << " " << camera->GetYaw() << endl;
    Vector3 shipPos = spaceship->GetTransform().GetPositionVector();

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_V)) {
        switch (viewFlag)
        {
        case 0:
            camera->SetPosition(shipPos);
            free = 0;
            dis = 3;
            break;
        case 1:
            camera->SetPosition(shipPos);
            free = 0;
            dis = 8;
            break;
        case 2:
            free = 1;
            break;
        default:
            break;
        }
        viewFlag++;
        if (viewFlag >= 3) viewFlag = 0;
    }
    if (!free)
    {
        spaceship->SetTransform(Matrix4::Translation(Vector3(camPos.x, camPos.y - dis, camPos.z)) * Matrix4::Rotation(camera->GetYaw() + 90, Vector3(0, 1, 0)));
    }
    frameFrustum.FromMatrix(projMatrix * viewMatrix);
    root->Update(dt);

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) { splitView = !splitView; }
    if (!splitView) { RenderScene(); }
    else if (splitView) {
        camera1->SetPosition(Vector3(camPos.x + 12.5, camPos.y + 2, camPos.z + 30));
        SplitRenderScene();
    }

    //autoPlay
    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_P)) {
        autoPlay = !autoPlay;
        camfree = !camfree;
        if (autoPlay) {
            delete camera;
            Vector3 tPos = solar->tianwang->GetWorldTransform().GetPositionVector();
            camera = new Camera(-10.0f, 0.0f, Vector3(tPos.x, tPos.y + 200, tPos.z));
            scene1 = false;
            scene2 = false;
            scene3 = false;
            scene4 = false;
        }
    }
    if (autoPlay)
    {
        AutoScene(dt);
    }
    else if (!autoPlay) { playTime = 0.0f; }

}

void URenderer::BuildNodeLists(SceneNode* from) {
    if (frameFrustum.InsideFrustum(*from) || from->GetCameraDistance() <= 50.0f) {
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

void URenderer::SortNodeLists() {
    std::sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);
    std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void URenderer::DrawNodes() {
    for (const auto& i : nodeList) {
        DrawNode(i);
    }
    for (const auto& i : transparentNodeList) {
        DrawNode(i);
    }
}

void URenderer::DrawNode(SceneNode* n) {
    if (n->GetMesh()) {
        if (n == soilring)
        {
            return;
        }
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
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, n->GetBump());
            glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
        }
        n->Draw(*this);
    }
}

void URenderer::DrawunLight(SceneNode* unLight) {
    if (unLight->GetMesh()) {
        BindShader(sceneShader);
        Matrix4 model = unLight->GetWorldTransform() * Matrix4::Scale(unLight->GetModelScale());
        UpdateShaderMatrices();
        glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, model.values);
        glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&unLight->GetColour());
        texture = unLight->GetTexture();
        glActiveTexture(GL_TEXTURE0);
        SetTextureRepeating(texture, true);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), texture);
        glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
        unLight->Draw(*this);
    }
}

void URenderer::ClearNodeLists() {
    transparentNodeList.clear();
    nodeList.clear();
}

void URenderer::DrawSkybox() {
    glDepthMask(GL_FALSE);

    BindShader(skyboxShader);
    UpdateShaderMatrices();

    quad->Draw();

    glDepthMask(GL_TRUE);
}

void URenderer::DrawMainScene() {
    BuildNodeLists(root);
    SortNodeLists();
    BindShader(lightShader);
    SetShaderLight(*light);

    glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

    DrawNodes();

}

void URenderer::RenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);
    viewMatrix = camera->BuildViewMatrix();
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
    DrawSkybox();
    DrawMainScene();
    DrawunLight(sun);
    DrawunLight(soilring);

}

void URenderer::SplitRenderScene() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    viewMatrix = camera->BuildViewMatrix();
    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, ((float)width / 2) / (float)height, 60.0f);
    glViewport(0, 0, width / 2, height);
    DrawSkybox();
    DrawMainScene();
    DrawunLight(sun);
    DrawunLight(soilring);

    projMatrix = Matrix4::Perspective(1.0f, 10000.0f, ((float)width) / (float)height, 60.0f);
    viewMatrix = camera1->BuildViewMatrix();
    glViewport(width / 2, 0, width, height);
    DrawSkybox();
    DrawMainScene();
    DrawunLight(sun);
    DrawunLight(soilring);

}
