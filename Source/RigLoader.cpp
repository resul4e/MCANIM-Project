#include "RigLoader.h"
#include "Rig.h"
#include "Joint.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

std::shared_ptr<Rig> RigLoader::LoadRig(std::filesystem::path _filePath)
{
	std::shared_ptr<Rig> rig = std::make_shared<Rig>();

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(_filePath.string(), aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	const std::shared_ptr<Joint> root = ExtractChild(scene->mRootNode);

	rig->SetRootJoint(root);

	return rig;
}

std::shared_ptr<Joint> RigLoader::ExtractChild(aiNode* _node)
{
	//We need the transformation matrix transposed.
	aiMatrix4x4 t = _node->mTransformation;
	glm::mat4x4 localTransform{ t.a1, t.b1, t.c1, t.d1 ,
								t.a2, t.b2, t.c2, t.d2 ,
								t.a3, t.b3, t.c3, t.d3 ,
								t.a4, t.b4, t.c4, t.d4 };

	//Create a new Joint with the correct name and at the correct position.
	std::shared_ptr<Joint> node = std::make_shared<Joint>(_node->mName.C_Str(), localTransform, JointType::ROTATIONAL, RotationalType::BALLSOCKET);

	for (size_t i = 0; i < _node->mNumChildren; i++)
	{
		//Call this function recursively for each child and add it to our children.
		std::shared_ptr<Joint> child = ExtractChild(_node->mChildren[i]);
		node->AddJoint(child);
	}

	return node;
}
