#include "TextureManager.h"

using namespace StringUtility;

TextureManager* TextureManager::instance = nullptr;

uint32_t TextureManager::kSRVIndexTop = 1;


TextureManager* TextureManager::GetInstance() {
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) {
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;
	textureDatas.reserve(DirectXCommon::kMaxSRVCount);
}


void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
}

void TextureManager::LoadTexture(const std::string& filePath) {

	//読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		return;
	}

	assert(srvManager_->Max());

	//テクスチャファイル // byte関連
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップ　//拡大縮小で使う
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));
	
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	
	//textureDatas.resize(textureDatas.size() + 1);いらない？

	//最後尾を取得
	TextureData& textureData = textureDatas[filePath];


	textureData.srvIndex = srvManager_->Allocate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);
	textureData.metadata = metadata;
	textureData.resoource = dxCommon_->CreateTextureResource(textureData.metadata);
	dxCommon_->UploadTextureData(textureData.resoource, mipImages);


	//SRVを作成するDescriptorHeap場所決め
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvHandleCPU = dxCommon_->GetSRVCPUDescriptorHandle(srvIndex);
	textureData.srvHandleGPU = dxCommon_->GetSRVGPUDescriptorHandle(srvIndex);
	

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	
	//SRVの生成
	dxCommon_->GetDevice()->CreateShaderResourceView(textureData.resoource.Get(), &srvDesc, textureData.srvHandleCPU);

}

//uint32_t TextureManager::GetTextureIndexByFilePath(const std::string filePath) {
//	//読み込み済みテクスチャを検索
//	if (textureDatas.contains(filePath)) {
//		TextureData& textureData = textureDatas[filePath];
//		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(),textureDatas.end()));
//		return textureIndex;
//	}
//
//	assert(0);
//	return 0;
//}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {
	assert(srvManager_->Max());

	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}

const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {
	assert(srvManager_->Max());

	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;
}

uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {
	assert(srvManager_->Max());

	TextureData& textureData = textureDatas[filePath];
	return textureData.srvIndex;
}