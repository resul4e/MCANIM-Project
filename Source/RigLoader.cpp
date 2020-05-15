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

	std::shared_ptr<Joint> root = std::make_shared<Joint>(nullptr);
	
	rig->SetRootJoint(root);
	
	return rig;
}
