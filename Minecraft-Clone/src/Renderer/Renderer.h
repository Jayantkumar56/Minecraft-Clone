

#pragma once

#include "Quirk.h"

#include "Mesh.h"

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