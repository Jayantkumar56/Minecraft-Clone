

#pragma once

#include "Quirk.h"

struct Mesh {
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> UV;
	std::vector<uint32_t>  Indices;
};

class CustomRenderer {
public:
	static void InitRenderer();
	static void BeginScene(const glm::mat4& projectionView, const glm::vec3& cameraPos);
	static void SubmitMesh(Mesh& mesh, glm::mat4& modelMat);
	static void EndScene();

private:
	struct SceneData {
		// camera data
		glm::mat4 ProjectionViewMatrix;
		glm::vec3 CameraPosition;

		// general mesh shader
		Quirk::Ref<Quirk::Shader>       MeshShader;
		Quirk::Ref<Quirk::VertexArray>  MeshVertexArray;
		Quirk::Ref<Quirk::VertexBuffer> MeshPositionBuffer;
		Quirk::Ref<Quirk::VertexBuffer> MeshNormalBuffer;
		Quirk::Ref<Quirk::VertexBuffer> MeshUVBuffer;

		// for light sources
		Quirk::Ref<Quirk::Shader> LightSourceShader;
	};

	static SceneData s_SceneData;
};