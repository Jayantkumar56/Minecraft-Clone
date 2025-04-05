

#include "Renderer.h"

CustomRenderer::SceneData CustomRenderer::s_SceneData;

void CustomRenderer::InitRenderer() {
	Quirk::RenderCommands::EnableBlending();

	s_SceneData.MeshShader        = Quirk::ShaderLibrary::LoadShader("assets/Shaders/Mesh.glsl");
	s_SceneData.LightSourceShader = Quirk::ShaderLibrary::LoadShader("assets/Shaders/emissive_mesh.glsl");

	s_SceneData.MeshVertexArray = Quirk::VertexArray::Create();

	s_SceneData.MeshPositionBuffer = Quirk::VertexBuffer::Create();
	s_SceneData.MeshPositionBuffer->SetLayout({ { Quirk::ShaderDataType::Float3,  "a_Position" } });
	s_SceneData.MeshVertexArray->AddVertexBuffer(s_SceneData.MeshPositionBuffer);

	s_SceneData.MeshNormalBuffer = Quirk::VertexBuffer::Create();
	s_SceneData.MeshNormalBuffer->SetLayout({ { Quirk::ShaderDataType::Float3,  "a_Normal" } });
	s_SceneData.MeshVertexArray->AddVertexBuffer(s_SceneData.MeshNormalBuffer);

	s_SceneData.MeshUVBuffer = Quirk::VertexBuffer::Create();
	s_SceneData.MeshUVBuffer->SetLayout({ { Quirk::ShaderDataType::Float2,  "a_TexCoord" } });
	s_SceneData.MeshVertexArray->AddVertexBuffer(s_SceneData.MeshUVBuffer);
}

void CustomRenderer::BeginScene(const glm::mat4& projectionView, const glm::vec3& cameraPos) {
	s_SceneData.ProjectionViewMatrix = projectionView;
	s_SceneData.CameraPosition       = cameraPos;
}

void CustomRenderer::SubmitMesh(const Mesh& mesh, Quirk::Ref<Quirk::Texture2D> spriteSheetTexture) {
	if (!mesh.Positions.size()) return;

	s_SceneData.MeshVertexArray->Bind();

	const void* data  = (const void*)mesh.Positions.data();
	uint32_t dataSize = (uint32_t)mesh.Positions.size() * sizeof(mesh.Positions[0]);
	s_SceneData.MeshPositionBuffer->UploadData(data, dataSize);

	data     = (const void*)mesh.Normals.data();
	dataSize = (uint32_t)mesh.Normals.size() * sizeof(mesh.Normals[0]);
	s_SceneData.MeshNormalBuffer->UploadData(data, dataSize);

	data     = (const void*)mesh.UV.data();
	dataSize = (uint32_t)mesh.Normals.size() * sizeof(mesh.UV[0]);
	s_SceneData.MeshUVBuffer->UploadData(data, dataSize);

	s_SceneData.MeshShader->Bind();
	s_SceneData.MeshShader->UploadUniform("u_ViewProjection", s_SceneData.ProjectionViewMatrix);

	spriteSheetTexture->Bind(0);
	int32_t  samplers      = 0;
	uint32_t samplersCount = 1;
	s_SceneData.MeshShader->UploadUniform("u_Texture", &samplers, samplersCount);

	uint32_t vertCount = (uint32_t)mesh.Positions.size();
	Quirk::RenderCommands::DrawVertices(vertCount);
}

void CustomRenderer::EndScene() {

}
