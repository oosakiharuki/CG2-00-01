#pragma once
#include "MyMath.h"
#include "ParticleCommon.h"

class Particle{
public:
	void Initialize(ParticleCommon* particleCommon, const std::string& fileName);
	//void ManagerInitialize(const std::string& directorypath, const std::string& fileName);
	void Update();
	void Draw();

	//void SetModel(Model* model) { this->model = model; }
	//void SetModelFile(const std::string& filePath);

	//void SetScale(const Vector3& scale) { transform.scale = scale; }
	//void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	//void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	//void SetCamera(Camera* camera) { this->camera = camera; }

	//const Vector3& GetScale() const { return transform.scale; }
	//const Vector3& GetRotate() const { return transform.rotate; }
	//const Vector3& GetTranslate()const { return transform.translate; }



	//static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

private:
	ParticleCommon* particleCommon = nullptr;

	
	std::string fileName;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;


	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;




	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;
	TransformationMatrix* wvpData = nullptr;

	//ライト用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightSphereResource;


	//マテリアルにデータを書き込む
	DirectionalLight* directionalLightSphereData = nullptr;


	static const uint32_t kNumInstance = 10;

	Transform transform[kNumInstance];
	Transform cameraTransform;


	Transform transformL;

	//Model* model = nullptr;
	Camera* camera = nullptr;

	ModelData modelData;

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
};