#include "Tree.h"
#include "../nclgl/OGLRenderer.h"

Tree::Tree()
{
	SetTransform(Matrix4::Translation(Vector3(6450, 125.0f, 6325)));

	srand(unsigned(time(0)));
	SceneNode* tree;
	for (int i = 0; i < 15; i++)
	{
		tree = new SceneNode();
		tree->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		tree->SetTransform(Matrix4::Translation(Vector3((rand() % 3000 - 1500), 0.0f, (rand() % 3000 - 1500))));
		tree->SetModelScale(Vector3(30.0f, 30.0f, 30.0f));
		tree->SetBoundingRadius(30.0f);
		mesh = Mesh::LoadFromMeshFile("Starfield/Tree.msh");
		tree->SetMesh(mesh);
		tree->SetSubMesh(1);
		material = new MeshMaterial("Starfield/Tree.mat");
		for (int j = 0; j < mesh->GetSubMeshCount(); ++j) {
			const MeshMaterialEntry* matEntry = material->GetMaterialForLayer(j);
			const string* filename = nullptr;
			matEntry->GetEntry("Diffuse", &filename);
			string path = TEXTUREDIR + *filename;
			GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
			tree->SetTextures(texID);
		}
		AddChild(tree);
	}

}

void Tree::Update(float dt)
{
	/*for (int i = 0; i < TREE_NUM; i++) {
		Vector3 treePos = children[i]->GetTransform().GetPositionVector();
		children[i]->SetTransform(Matrix4::Translation(Vector3(treePos.x + d * 10, treePos.y + d * 10, treePos.z + d * 10)));
		d += x;
		if (d >= 3 || d <= -3)
		{
			x *= -1;
		}
	}*/
	SceneNode::Update(dt);

}
