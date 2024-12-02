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
	void Initialize(SpriteCommon* spriteCommon,std::string textureFilePath);
	void Update();
	void Draw();


	const Vector2& GetPosition()const { return position; }
	void SetPosition(const Vector2& position) { this->position = position; }

	float GetRotate() { return rotation; }
	void SetRotate(float rotation) { this->rotation = rotation; }

	const Vector4& GetColor()const { return materialData->color; }
	void SetColor(const Vector4& color) { materialData->color = color; }

	const Vector2& GetSize()const { return size; }
	void SetSize(const Vector2& size) { this->size = size; }

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

	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	
	Transform uvTransform{
	{ 1.0f,1.0f,1.0f },
	{ 0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f }
	};


	//座標
	Vector2 position = { 0.0f,0.0f };
	
	float rotation = 0.0f;

	Vector2 size = { 100.0f,100.0f };


	uint32_t textureIndex = 0;
};
