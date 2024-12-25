#include "Object3d.h"
#include "Object3dCommon.h"
#include "externals/imgui/imgui.h"
#include "TextureManager.h"

#include <fstream>
#include <sstream>
#include "ModelManager.h"

using namespace MyMath;

void Object3d::Initialize(Object3dCommon* object3dCommon_) {
	this->object3dCommon = object3dCommon_;	
	this->camera = object3dCommon->GetDefaultCamera();
	wvpResource = object3dCommon->GetDirectXCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	
	
	wvpData->World = MakeIdentity4x4();
	wvpData->WVP= MakeIdentity4x4();

	//ライト用のリソース
	directionalLightSphereResource = object3dCommon->GetDirectXCommon()->CreateBufferResource(sizeof(DirectionalLight));
	//書き込むためのアドレス
	directionalLightSphereResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightSphereData));
	//色の設定
	directionalLightSphereData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightSphereData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightSphereData->intensity = 1.0f;

	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f}, {0.0f,4.0f,-10.0f} };

	transformL = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };




}

void Object3d::Update() {

	//モデル
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);	
	Matrix4x4 WorldViewProjectionMatrix;

	if (camera) {
		Matrix4x4 projectionMatrix = camera->GetViewProjectionMatrix();
		WorldViewProjectionMatrix = Multiply(worldMatrix, projectionMatrix);
	}
	else {
		WorldViewProjectionMatrix = worldMatrix;
	}

	wvpData->World = worldMatrix;
	wvpData->WVP = WorldViewProjectionMatrix;

	directionalLightSphereData->direction = Normalize(directionalLightSphereData->direction);


}


void Object3d::Draw() {
	//モデル
	object3dCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	object3dCommon->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightSphereResource->GetGPUVirtualAddress());
	if (model) {
		model->Draw();
	}
}

void Object3d::SetModelFile(const std::string& filePath) {
	model = ModelManager::GetInstance()->FindModel(filePath);
}