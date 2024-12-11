#pragma once
#include "MyMath.h"

//ComPtr
#include <wrl.h>
#include "d3d12.h"

class Object3dCommon;

class Object3d
{
public:
	void Initialize(Object3dCommon* object3dCommon_);
	void Update();
	void Draw();


	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	Object3dCommon* object3dCommon = nullptr;
	ModelData modelData;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;


	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* wvpData = nullptr;


	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//ライト用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightSphereResource;


	//マテリアルにデータを書き込む
	DirectionalLight* directionalLightSphereData = nullptr;


	Transform transform;
	Transform cameraTransform;


	Transform transformL;
};