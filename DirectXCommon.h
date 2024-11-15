#pragma once
#include <d3d12.h>
#include<dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <array>

#include <dxcapi.h>
#include <format>

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#include "Logger.h"
#include "StringUtility.h"

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

class DirectXCommon {
public:
	void Initialize();
	void Device();//デバイスの初期化
	void Commond();//コマンド関連
	void SwapChain();//スワップチェーン
	Microsoft::WRL::ComPtr<ID3D12Resource> ZBuffer(Microsoft::WRL::ComPtr <ID3D12Device> device, int32_t width, int32_t height);//
	void DescriptorHeap();
	void RTV();

	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);



	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, size_t sizeInBytes);
	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	Microsoft::WRL::ComPtr<ID3D12Resource> CrateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);
	void UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);


	void DSV();//深度ステンシルビュー

	void Fence();

	void ViewPort();
	void Scissor();
	void DXC();
	void ImGui();

private:
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	WinApp* winApp_ = nullptr;
	//コマンド
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	Microsoft::WRL::ComPtr < ID3D12CommandAllocator> commandAllocator = nullptr;

	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList = nullptr;

	//スワップチェイン
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResource[2] = { nullptr };

	//バッファ
	D3D12_RESOURCE_DESC resourceDesc{};
	D3D12_HEAP_PROPERTIES heapProperties{};
	D3D12_CLEAR_VALUE depthClearValue{};
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;



	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle;

	uint32_t MaxRtvHandle = 2;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];







	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> rtvDescriptorHeap;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> srvDescriptorHeap;
	const uint32_t descriptorSizeSRV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t descriptorSizeRTV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	const uint32_t descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> dsvDescriptorHeap;

	D3D12_DEPTH_STENCIL_VIEW_DESC dscDesc{};

	Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDesciptors, bool shaderVisible);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> descriptorHeap, uint32_t descriptorSize, uint32_t index);

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;


	//DXC
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;


};