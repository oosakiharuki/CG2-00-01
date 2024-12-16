#pragma once
#include <string>
#include "externals/DirectXTex/DirectXTex.h"
#include <wrl.h>
#include <d3d12.h>

#include "DirectXCommon.h"

class TextureManager {
public:
	static TextureManager* GetInstance();
	void Initialize(DirectXCommon* dxCommon);
	void Finalize();

	void LoadTexture(const std::string& filePath);

	uint32_t GetTextureIndexByFilePath(const std::string filePath);

	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(uint32_t textureIndex);

	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);
private:
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = default;
	TextureManager& operator=(TextureManager&) = default;

	struct TextureData {
		std::string filePath; // (resource/○○.png)?
		DirectX::TexMetadata metadata; //width,height
		Microsoft::WRL::ComPtr<ID3D12Resource>resoource; // テクスチャリソース
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	std::vector<TextureData> textureDatas;


	static uint32_t kSRVIndexTop;

	DirectXCommon* dxCommon_ = nullptr;
};