#pragma once
#include "DirectXCommon.h"

class SrvManager{
public:
	void Initialize(DirectXCommon* dxCommon);
	uint32_t Allocate();
	bool Max();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);
	
	//texture用
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	void CreateSRVforStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

private:
	DirectXCommon* directXCommon = nullptr;

	//最大SRV数
	static const uint32_t kMaxSRVCount;
	//
	uint32_t descriptorSize;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

	uint32_t useIndex = 0;
};