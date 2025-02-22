#include "Particle.h"
#include "TextureManager.h"
#include "ParticleManager.h"
#include "Camera.h"
#include <fstream>
#include <sstream>


using namespace MyMath;

void Particle::Initialize(ParticleCommon* ParticleCommon, const std::string& fileName) {
	this->particleCommon = ParticleCommon;
	this->camera = particleCommon->GetDefaultCamera();
	this->fileName = fileName;

	modelData = ParticleManager::GetInstance()->GetModelData(fileName);

	vertexResource = particleCommon->GetDxCommon()->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);



	//wvpResource = particleCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	wvpResource = ParticleManager::GetInstance()->GetResource(fileName);
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		wvpData[index].World = MakeIdentity4x4();
		wvpData[index].WVP = MakeIdentity4x4();
	}

	//wvpResource = particleCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	//wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));	
	//wvpData->World = MakeIdentity4x4();
	//wvpData->WVP = MakeIdentity4x4();


	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());










	//Particle用マテリアル
	//マテリアル用のリソース
	materialResource = particleCommon->GetDxCommon()->CreateBufferResource(sizeof(Material));
	//書き込むためのアドレス
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//色の設定
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = true;
	materialData->uvTransform = MakeIdentity4x4();

	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture(modelData.material.textureFilePath);
	modelData.material.textureIndex = TextureManager::GetInstance()->GetSrvIndex(modelData.material.textureFilePath);


	//ライト用のリソース
	directionalLightSphereResource = particleCommon->GetDxCommon()->CreateBufferResource(sizeof(DirectionalLight));
	//書き込むためのアドレス
	directionalLightSphereResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightSphereData));
	//色の設定
	directionalLightSphereData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightSphereData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightSphereData->intensity = 1.0f;

	//transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };

	for (uint32_t index = 0; index < kNumInstance; ++index) {
		transform[index].scale = { 1.0f,1.0f,1.0f };
		transform[index].rotate = { 0.0f,3.0f,0.0f };
		transform[index].translate = { index * 0.1f,index * 0.1f,index * 0.1f };
	}




	cameraTransform = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f}, {0.0f,4.0f,-10.0f} };

	transformL = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };

}

void Particle::Update() {
	//モデル
	//Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//Matrix4x4 WorldViewProjectionMatrix;
	//if (camera) {
	//	Matrix4x4 projectionMatrix = camera->GetViewProjectionMatrix();
	//	WorldViewProjectionMatrix = Multiply(worldMatrix, projectionMatrix);
	//}
	//else {
	//	WorldViewProjectionMatrix = worldMatrix;
	//}
	//wvpData->World = worldMatrix;
	//wvpData->WVP = WorldViewProjectionMatrix;

	for (uint32_t index = 0; index < kNumInstance; ++index) {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform[index].scale, transform[index].rotate, transform[index].translate);
		Matrix4x4 WorldViewProjectionMatrix;

		if (camera) {
			Matrix4x4 projectionMatrix = camera->GetViewProjectionMatrix();
			WorldViewProjectionMatrix = Multiply(worldMatrix, projectionMatrix);
		}
		else {
			WorldViewProjectionMatrix = worldMatrix;
		}

		wvpData[index].World = worldMatrix;
		wvpData[index].WVP = WorldViewProjectionMatrix;
	}
	
	directionalLightSphereData->direction = Normalize(directionalLightSphereData->direction);



}

void Particle::Draw() {
	
	particleCommon->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	particleCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress()); //rootParameterの配列の0番目 [0]
	particleCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	particleCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData.material.textureFilePath));
	particleCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightSphereResource->GetGPUVirtualAddress());

	//4のやつ particle専用
	particleCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(4, ParticleManager::GetInstance()->GetSrvHandleGPU(fileName));

	particleCommon->GetDxCommon()->GetCommandList()->ClearDepthStencilView(particleCommon->GetDxCommon()->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	particleCommon->GetDxCommon()->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()),kNumInstance, 0, 0);
}