#pragma once
#include "Mesh.h"

using namespace std;

class HeightMap : public Mesh {
public:
    HeightMap(const string& name);
    ~HeightMap(void) {};
    Vector3 GetHeightmapSize() const { return heightmapSize; }
protected:
    Vector3 heightmapSize;
};