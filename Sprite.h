#pragma once
//#include "Vector2.h"
//#include "Vector3.h"
//#include "Vector4.h"
#include "MyMath.h"
//ComPtr
#include <wrl.h>

#include "DirectXCommon.h"

class SpriteCommon;

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];//枠確保用06-01 9
	Matrix4x4 uvTransform;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

class Sprite{
public:
	void Initialize(SpriteCommon* spriteCommon);
	void Update();
	void Draw();

	void Imgui();

private:
	SpriteCommon* spriteCommon_ = nullptr;


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;

	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	//頂点バッファービュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;


	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	//マテリアルにデータを書き込む
	Material* materialData = nullptr;


	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;

	TransformationMatrix* transformationMatrixData = nullptr;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	
	Transform uvTransformSprite{
	{ 1.0f,1.0f,1.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f }
	};

	bool IsSprite = true;
};
