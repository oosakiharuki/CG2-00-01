#include "ParticleManager.h"
#include <cassert>

using namespace MyMath;

using namespace StringUtility;

ParticleManager* ParticleManager::instance = nullptr;

uint32_t ParticleManager::kSRVIndexTop = 1;

ParticleManager* ParticleManager::GetInstance() {
	if (instance == nullptr) {
		instance = new ParticleManager();
	}
	return instance;
}

void ParticleManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) {
	particleCommon = new ParticleCommon(); 
	this->srvManager = srvManager;
	particleCommon->Initialize(dxCommon);
}

void ParticleManager::Finalize() {
	delete particleCommon;
	delete instance;
	instance = nullptr;
}

//void ParticleManager::LoadModel(const std::string& filePath) {
//	if (particles.contains(filePath)) {
//		return;
//	}
//
//	std::unique_ptr<Particle> particle = std::make_unique <Particle> ();
//	particle->ManagerInitialize("resource", filePath);//model,file名,OBJ本体
// 
//	particles.insert(std::make_pair(filePath, std::move(particle)));
//
//}

//Particle* ParticleManager::FindModel(const std::string& filePath) {
//	if (particles.contains(filePath)) {
//		return particles.at(filePath).get();
//	}
//
//	//ファイル一致なし
//	return nullptr;
//}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath) {
	//読み込み済み
	if (particleGroups.contains(name)) {
		return;
	}

	assert(srvManager->Max());

	////テクスチャファイル // byte関連
	//DirectX::ScratchImage image{};
	//std::wstring filePathW = ConvertString(textureFilePath);
	//HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	//assert(SUCCEEDED(hr));

	////ミップマップ　//拡大縮小で使う
	//DirectX::ScratchImage mipImages{};
	//hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	//assert(SUCCEEDED(hr));

	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();


	ParticleGroup& particleG = particleGroups[name];


	particleG.textureFile = textureFilePath;

	//モデルでするとき
	//particleG.modelData = LoadObjFile("resource", "fence.obj");

	particleG.modelData.vertices.push_back({ {1.0f,1.0f,0.0f,1.0f},{0.0f,0.0f},{0.0f,0.0f,1.0f} });
	particleG.modelData.vertices.push_back({ {-1.0f,1.0f,0.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f,1.0f} });
	particleG.modelData.vertices.push_back({ {1.0f,-1.0f,0.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f,1.0f} });
	particleG.modelData.vertices.push_back({ {1.0f,-1.0f,0.0f,1.0f},{0.0f,1.0f},{0.0f,0.0f,1.0f} });
	particleG.modelData.vertices.push_back({ {-1.0f,1.0f,0.0f,1.0f},{1.0f,0.0f},{0.0f,0.0f,1.0f} });
	particleG.modelData.vertices.push_back({ {-1.0f,-1.0f,0.0f,1.0f},{1.0f,1.0f},{0.0f,0.0f,1.0f} });

	particleG.modelData.material.textureFilePath = textureFilePath;


	
	//particleG.metadata = metadata;
	//particleG.resource = particleCommon->GetDxCommon()->CreateTextureResource(particleG.metadata);
	//particleCommon->GetDxCommon()->UploadTextureData(particleG.resource, mipImages);


	particleG.kNumInstance = 10;

	particleG.resource = particleCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix) * particleG.kNumInstance);

	//TransformationMatrix* instancingData = nullptr;
	//particleG.resource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	//for (uint32_t index = 0; index < particleG.kNumInstance; ++index) {
	//	instancingData[index].World = MakeIdentity4x4();
	//	instancingData[index].WVP = MakeIdentity4x4();
	//}


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = particleG.kNumInstance;
	srvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	particleG.srvIndex = srvManager->Allocate();
	particleG.srvHandleCPU = srvManager->GetCPUDescriptorHandle(particleG.srvIndex);
	particleG.srvHandleGPU = srvManager->GetGPUDescriptorHandle(particleG.srvIndex);
	

	//SRVの生成
	particleCommon->GetDxCommon()->GetDevice()->CreateShaderResourceView(particleG.resource.Get(), &srvDesc, particleG.srvHandleCPU);

}

D3D12_GPU_DESCRIPTOR_HANDLE  ParticleManager::GetSrvHandleGPU(const std::string filePath) {
	assert(srvManager->Max());

	ParticleGroup& particleG = particleGroups[filePath];
	return particleG.srvHandleGPU;
}

ModelData ParticleManager::GetModelData(const std::string filePath) {
	assert(srvManager->Max());

	ParticleGroup& particleG = particleGroups[filePath];
	return particleG.modelData;
}

std::string ParticleManager::GetTextureHandle(const std::string filePath) {
	assert(srvManager->Max());

	ParticleGroup& particleG = particleGroups[filePath];
	return particleG.textureFile;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ParticleManager::GetResource(const std::string filePath) {
	assert(srvManager->Max());

	ParticleGroup& particleG = particleGroups[filePath];
	return particleG.resource;
}