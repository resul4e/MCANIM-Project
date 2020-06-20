#include "LinearBlendSkinning.h"

#include "Rig.h"
#include "Model.h"
#include "Joint.h"

void LinearBlendSkinning::Skin(const Rig& _rig, Mesh& _mesh)
{
	//get references to the positions and normals we are going to use.
	std::vector<glm::vec3>& inPositions = _mesh.positions;
	std::vector<glm::vec3>& inNormals = _mesh.normals;

	for (const Bone& bone : _mesh.m_bones)
	{
		std::shared_ptr<Joint> joint = _rig.GetJoint(bone.m_name);
		if (joint == nullptr) continue;

		const glm::mat4& offsetMatrix = bone.m_offsetMatrix;
		glm::mat4 globalTransform = joint->GetGlobalTransform();
		glm::mat4 finalTransform = globalTransform * offsetMatrix;

		for (const VertexWeight& vertexWeight : bone.m_weights)
		{
			const int& vIndex = vertexWeight.m_vertexIndex;
			const float& vWeight = vertexWeight.m_weight;
			glm::vec4 v = glm::vec4(inPositions[vIndex], 1);
			_mesh.animatedPositions[vIndex] += glm::vec3(finalTransform * v) * vWeight;
			_mesh.animatedNormals[vIndex] += glm::mat3(finalTransform) * inNormals[vertexWeight.m_vertexIndex] * vWeight;
		}
	}
}
