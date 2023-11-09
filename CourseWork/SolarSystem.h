#pragma once
#include "../nclgl/SceneNode.h"
#include "../nclgl/OBJMesh.h"
#include <string>

using namespace std;

class SolarSystem : public SceneNode
{
public:
    SolarSystem();
    ~SolarSystem(void) {};
    void Update(float dt) override;
    Vector3 getEarthPosiotion() { return earth->GetWorldTransform().GetPositionVector(); };
    vector<Matrix4> GetSceneTransforms() { return sceneTransforms; }

protected:
    Mesh* m;
    GLuint texture;

    //planet
    SceneNode* sun; //0
    SceneNode* mercury; //1
    SceneNode* gold; //2
    SceneNode* earth; //3
    SceneNode* earthCloud; //3
    SceneNode* moon; //3.5
    SceneNode* fire; //4
    SceneNode* wood; //5
    SceneNode* soil; //6
    SceneNode* soilring; //6
    SceneNode* tianwang; //7
    SceneNode* haiwang; //8

    SceneNode* mercuryR; //1
    SceneNode* goldR; //2
    SceneNode* earthR; //3
    SceneNode* fireR; //4
    SceneNode* woodR; //5
    SceneNode* soilR; //6
    SceneNode* tianwangR; //7
    SceneNode* haiwangR; //8

    vector<Matrix4> sceneTransforms;

};
