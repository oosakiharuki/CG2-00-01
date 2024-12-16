//#include<Windows.h>//winapp
#include <cstdint>
//#include <string>
#include<format>

#include<cassert>

#include <dxgidebug.h>

//#include <dxcapi.h>





#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "MyMath.h"
using namespace MyMath;

#pragma comment(lib,"dxguid.lib")


#pragma comment(lib,"dxcompiler.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "Input.h"
//#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResorceLeakChecker.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#include "Model.h"
#include "ModelManager.h"

using namespace Logger;
using namespace StringUtility;

//std::string str0{ "STRING!!!" };
//
//std::string str1{ std::to_string(10) };


struct Sphere {
	Vector3 center;
	float radius;
};


VertexData AddVert(const VertexData& v1, const VertexData& v2) {
	VertexData result{};

	result.position.x = v1.position.x + v2.position.x;
	result.position.y = v1.position.y + v2.position.y;
	result.position.z = v1.position.z + v2.position.z;
	result.position.s = v1.position.s + v2.position.s;
	result.texcoord.x = v1.texcoord.x + v2.texcoord.x;
	result.texcoord.y = v1.texcoord.y + v2.texcoord.y;
	return result;
}

void DrawSphere(VertexData* vertexDataSphere) {

	const uint32_t kSubdivision = 16;

	float pi = float(M_PI);

	const float kLonEvery = pi * 2.0f / float(kSubdivision);
	const float kLatEvery = pi / float(kSubdivision);


	VertexData vertexDataBkaraA[kSubdivision]{};

	VertexData vertexDataCkaraA[kSubdivision]{};

	VertexData vertexDataDkaraA[kSubdivision][kSubdivision]{};

	VertexData vertexDataDkaraC[kSubdivision]{};
	VertexData vertexDataDkaraB[kSubdivision]{};


	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;//緯度 シ－タ

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {

			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;//経度　ファイ


			VertexData vertA{};
			vertA.position =
			{
				std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon),
				1.0f
			};
			vertA.texcoord =
			{
				float(lonIndex) / float(kSubdivision),
				1.0f - float(latIndex) / float(kSubdivision)
			};
			vertA.normal = {
				0.0f,0.0f,-1.0f 
			};


			VertexData vertB{};
			vertB.position =
			{
				std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon)
				,1.0f
			};
			vertB.texcoord =
			{
				float(lonIndex) / float(kSubdivision),
				1.0f - float(latIndex + 1) / float(kSubdivision)
			};
			vertB.normal = {
				0.0f,0.0f,-1.0f
			};


			VertexData vertC{};
			vertC.position =
			{
				std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery),
				1.0f
			};
			vertC.texcoord =
			{
				float(lonIndex + 1) / float(kSubdivision),
				1.0f - float(latIndex) / float(kSubdivision)
			};
			vertC.normal = {
				0.0f,0.0f,-1.0f
			};


			VertexData vertD{};
			vertD.position =
			{
				std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery),
				1.0f
			};
			vertD.texcoord =
			{
				float(lonIndex + 1) / float(kSubdivision),
				1.0f - float(latIndex + 1) / float(kSubdivision)
			};
			vertD.normal = {
				0.0f,0.0f,-1.0f
			};




			//最初点
			vertexDataSphere[start + 0] = vertA;
			vertexDataSphere[start + 1] = vertB;
			vertexDataSphere[start + 2] = vertC;

			vertexDataSphere[start + 3] = vertC;
			vertexDataSphere[start + 4] = vertB;
			vertexDataSphere[start + 5] = vertD;

		}

	}


	for (uint32_t index = 0; index < kSubdivision * kSubdivision * 6; index++) {
		vertexDataSphere[index].normal.x = vertexDataSphere[index].position.x;
		vertexDataSphere[index].normal.y = vertexDataSphere[index].position.y;
		vertexDataSphere[index].normal.z = vertexDataSphere[index].position.z;
	}

	
}

//Windowsアプリのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//旧WinApp
	D3DResourceLeakChecker leakCheck;

	WinApp* winApp_ = nullptr;

	winApp_ = new WinApp();
	winApp_->Initialize();

	Input* input_;
	input_ = new Input();
	//GetHInstance()GetHwnd()を入れず直接winAppのクラスのものを使える
	input_->Initialize(winApp_);


	//ShowWindow(hwnd, SW_SHOW);

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->SetWinApp(winApp_);
	dxCommon->Initialize();

	TextureManager::GetInstance()->Initialize(dxCommon);

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	std::vector<Sprite*> sprites;

	for (uint32_t i = 0; i < 1; ++i) {
		Sprite* sprite = new Sprite();
		//if (i == 1 || i == 3) {
		//	sprite->Initialize(spriteCommon, "resource/monsterBall.png");
		//}
		//else {
		sprite->Initialize(spriteCommon, "resource/uvChecker.png");
		//}
		Vector2 position[5] = {};
		position[i].x += i * 200.0f;
		sprite->SetPosition(position[i]);

		sprites.push_back(sprite);
	}


	ModelManager::GetInstance()->Initialize(dxCommon);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	Object3dCommon* object3dCommon = nullptr;
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);


	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);


	//Model* model = nullptr;
	//model = new Model();
	//model->Initialize(modelCommon);

	std::vector <Object3d*> objects;


	Vector3 position[2] = {};

	for (uint32_t i = 0; i < 2; ++i) {

		Object3d* object3d = new Object3d();
		//object3d->SetModel(model);
	
		if (i == 0) {
			object3d->SetModelFile("plane.obj");
			object3d->Initialize(object3dCommon);
		}
		else {
			object3d->SetModelFile("axis.obj");
			object3d->Initialize(object3dCommon);
		}

		position[i].x += i * 3.0f;
		object3d->SetTranslate(position[i]);

		objects.push_back(object3d);
	}


	//三角
	//Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(VertexData) * 6);


	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	//vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	//vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;

	//vertexBufferView.StrideInBytes = sizeof(VertexData);


	//Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource = dxCommon->CreateBufferResource(sizeof(Matrix4x4));

	//Matrix4x4* wvpData = nullptr;

	//wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//*wvpData = MakeIdentity4x4();

	//VertexData* vertexData = nullptr;

	//vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	////leftTop
	//vertexData[0].position = { -0.5f, -0.5f,0.0f,1.0f };
	//vertexData[0].texcoord = { 0.0f,1.0f };

	////Top
	//vertexData[1].position = { 0.0f, 0.5f,0.0f,1.0f };
	//vertexData[1].texcoord = { 0.5f,0.0f };

	////rightBottom
	//vertexData[2].position = { 0.5f, -0.5f,0.0f,1.0f };
	//vertexData[2].texcoord = { 1.0f,1.0f };



	////leftTop
	//vertexData[3].position = { -0.5f, -0.5f,0.5f,1.0f };
	//vertexData[3].texcoord = { 0.0f,1.0f };

	////Top
	//vertexData[4].position = { 0.0f, 0.0f,0.0f,1.0f };
	//vertexData[4].texcoord = { 0.5f,0.0f };

	////rightBottom
	//vertexData[5].position = { 0.5f, -0.5f,-0.5f,1.0f };
	//vertexData[5].texcoord = { 1.0f,1.0f };






	uint32_t SphereVertexNum = 16 * 16 * 6;

	//Sphere
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSphere = dxCommon->CreateBufferResource(sizeof(VertexData) * SphereVertexNum);


	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere{};

	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();

	vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * SphereVertexNum;

	vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResourceSphere = dxCommon->CreateBufferResource(sizeof(TransformationMatrix));

	TransformationMatrix* wvpDataSphere = nullptr;

	wvpResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataSphere));

	wvpDataSphere->World = MakeIdentity4x4();

	VertexData* vertexDataSphere = nullptr;

	vertexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere));


	//モデルの読み込み
	//ModelData modelData = LoadObjFile("resource", "plane.obj");

	//ModelData modelData = LoadObjFile("resource", "axis.obj");

	//ModelData modelData = LoadObjFile("resource", "multiMesh.obj");

	//ModelData modelData = LoadObjFile("resource", "teapot.obj");

	//static int modelChange = 0;



	//まだ	
	//ModelData modelData = LoadObjFile("resource", "multiMaterial.obj");
	//ModelData modelData = LoadObjFile("resource", "stanford-bunny.obj");


	////三角用マテリアル
	////マテリアル用のリソース
	//Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = dxCommon->CreateBufferResource(sizeof(Vector4));
	////マテリアルにデータを書き込む
	//Vector4* materialData = nullptr;
	////書き込むためのアドレス
	//materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	////色の設定
	//*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);




	////球体用マテリアル
	////マテリアル用のリソース
	//Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSphere = dxCommon->CreateBufferResource(sizeof(Material));
	////マテリアルにデータを書き込む
	//Material* materialDataSphere = nullptr;
	////書き込むためのアドレス
	//materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere));
	////色の設定
	//materialDataSphere->color =  Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	//materialDataSphere->enableLighting = true;
	//materialDataSphere->uvTransform = MakeIdentity4x4();









	Transform transformSphere{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };





	//float *inputMaterial[3] = { &materialData->x,&materialData->y,&materialData->z };
	//float* inputTransform[3] = { &transform.translate.x,&transform.translate.y,&transform.translate.z };
	//float* inputRotate[3] = { &transform.rotate.x,&transform.rotate.y,&transform.rotate.z };
	//float* inputScale[3] = { &transform.scale.x,&transform.scale.y,&transform.scale.z };


	//float* inputMaterialSphere[3] = { &materialDataSphere->color.x,&materialDataSphere->color.y,&materialDataSphere->color.z };
	//float* inputTransformSphere[3] = { &transformSphere.translate.x,&transformSphere.translate.y,&transformSphere.translate.z };
	//float* inputRotateSphere[3] = { &transformSphere.rotate.x,&transformSphere.rotate.y,&transformSphere.rotate.z };
	//float* inputScaleSphere[3] = { &transformSphere.scale.x,&transformSphere.scale.y,&transformSphere.scale.z };
	//bool textureChange = false;

	//float* inputMaterialModel[3] = { &materialDataModel->color.x,&materialDataModel->color.y,&materialDataModel->color.z };
	//float* inputTransformModel[3] = { &transformModel.translate.x,&transformModel.translate.y,&transformModel.translate.z };
	//float* inputRotateModel[3] = { &transformModel.rotate.x,&transformModel.rotate.y,&transformModel.rotate.z };
	//float* inputScaleModel[3] = { &transformModel.scale.x,&transformModel.scale.y,&transformModel.scale.z };
	//bool textureChange2 = false;


	//float* inputMateriallight[3] = { &directionalLightSphereData->color.x,&directionalLightSphereData->color.y,&directionalLightSphereData->color.z };
	//float* inputDirectionLight[3] = { &directionalLightSphereData->direction.x,&directionalLightSphereData->direction.y,&directionalLightSphereData->direction.z };
	//float* intensity = &directionalLightSphereData->intensity;

	//描画させるもの
	bool IsSphere = true;
	bool IsModel[2] = {true,true};
	bool IsSprite = true;




	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		if (winApp_->ProcessMessage()) {
			break;//whileを抜ける
		}
		else {
			//ゲームの処理
			
			input_->Update();
			
			if (input_->PushKey(DIK_0)) {
				OutputDebugStringA("Hit 0\n");
			}

			if (input_->TriggerKey(DIK_1)) {
				OutputDebugStringA("Hit 1\n");
			}


			Vector2 position;
			float rotation;
			Vector4 color;
			Vector2 size;

			for (Sprite* sprite : sprites) {
				sprite->Update();


				//position = sprite->GetPosition();
				//position.x += 0.1f;
				//position.y += 0.1f;
				//sprite->SetPosition(position);

				//rotation = sprite->GetRotate();
				//rotation += 0.01f;
				//sprite->SetRotate(rotation);

				color = sprite->GetColor();
				//color.x += 0.01f;
				//if (color.x > 1.0f) {
				//	color.x -= 1.0f;
				//}
				sprite->SetColor(color);

				size = sprite->GetSize();
				//size.x -= 1.1f;
				//size.y -= 1.1f;
				sprite->SetSize(size);
			}


			Vector3 positionOBJ;
			Vector3 rotationOBJ;
			Vector3 rotationOBJ2;
			Vector3 sizeOBJ;

			for (Object3d* object3d : objects) {
				object3d->Update();

				positionOBJ = object3d->GetTranslate();
				
				object3d->SetTranslate(positionOBJ);
				
				rotationOBJ = object3d->GetRotate();
				
				object3d->SetRotate(rotationOBJ);


				sizeOBJ = object3d->GetScale();

				object3d->SetScale(sizeOBJ);

			}
				
			rotationOBJ = objects[0]->GetRotate();
			rotationOBJ.y += 0.05f;
			objects[0]->SetRotate(rotationOBJ);

			rotationOBJ2 = objects[1]->GetRotate();
			rotationOBJ2.z += 0.1f;			
			objects[1]->SetRotate(rotationOBJ2);




			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			//三角
			//*wvpData = WorldViewProjectionMatrix;


			//球体

			//Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
			//Matrix4x4 WorldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));

			//wvpDataSphere->World = worldMatrixSphere;
			//wvpDataSphere->WVP = WorldViewProjectionMatrixSphere;
			//		
			//DrawSphere(vertexDataSphere);
		
		

			//開発用UIの処理
			//ImGui::ShowDemoWindow();

			//ここにテキストを入れられる
			ImGui::Text("ImGuiText");


			//ImGui::Text("Traiangle");
			//ImGui::InputFloat3("Material", *inputMaterial);
			//ImGui::SliderFloat3("SliderMaterial", *inputMaterial, 0.0f, 1.0f);

			//ImGui::InputFloat3("Vertex", *inputTransform);
			//ImGui::SliderFloat3("SliderVertex", *inputTransform, -5.0f, 5.0f);

			//ImGui::InputFloat3("Rotate", *inputRotate);
			//ImGui::SliderFloat3("SliderRotate", *inputRotate, -10.0f, 10.0f);

			//ImGui::InputFloat3("Scale", *inputScale);
			//ImGui::SliderFloat3("SliderScale", *inputScale, 0.5f, 5.0f);



			//if (ImGui::TreeNode("Sphere")) {
			//	ImGui::Checkbox("IsSphere", &IsSphere);

			//	if (IsSphere) {
			//		ImGui::InputFloat3("MaterialSphere", *inputMaterialSphere);
			//		ImGui::SliderFloat3("SliderMaterialSphere", *inputMaterialSphere, 0.0f, 1.0f);

			//		ImGui::InputFloat3("VertexSphere", *inputTransformSphere);
			//		ImGui::SliderFloat3("SliderVertexSphere", *inputTransformSphere, -5.0f, 5.0f);

			//		ImGui::InputFloat3("RotateSphere", *inputRotateSphere);
			//		ImGui::SliderFloat3("SliderRotateSphere", *inputRotateSphere, -10.0f, 10.0f);

			//		ImGui::InputFloat3("ScaleSphere", *inputScaleSphere);
			//		ImGui::SliderFloat3("SliderScaleSphere", *inputScaleSphere, 0.5f, 5.0f);

			//		ImGui::Checkbox("SphereTexture", &textureChange);
			//	}
			//	ImGui::TreePop();
			//}

			
			if (ImGui::TreeNode("Model_1")) {
				ImGui::Checkbox("IsModel", &IsModel[0]);
				if (IsModel) {

					positionOBJ = objects[0]->GetTranslate();
					ImGui::InputFloat3("VertexModel", &positionOBJ.x);
					ImGui::SliderFloat3("SliderVertexModel", &positionOBJ.x, -5.0f, 5.0f);
					objects[0]->SetTranslate(positionOBJ);


					rotationOBJ = objects[0]->GetRotate();
					ImGui::InputFloat3("RotateModel", &rotationOBJ.x);
					ImGui::SliderFloat3("SliderRotateModel", &rotationOBJ.x, -10.0f, 10.0f);

					objects[0]->SetRotate(rotationOBJ);


					sizeOBJ = objects[0]->GetScale();
					ImGui::InputFloat3("ScaleModel", &sizeOBJ.x);
					ImGui::SliderFloat3("SliderScaleModel", &sizeOBJ.x, 0.5f, 5.0f);

					objects[0]->SetScale(sizeOBJ);

					//ImGui::InputFloat3("MaterialModel", *);
					//ImGui::SliderFloat3("SliderMaterialModel", *inputMaterialModel, 0.0f, 1.0f);
					//ImGui::Checkbox("ModelTexture", &textureChange2);
				}
				ImGui::TreePop();
			}
			

			if (ImGui::TreeNode("Model_2")) {
				ImGui::Checkbox("IsModel", &IsModel[1]);
				if (IsModel[1]) {

					positionOBJ = objects[1]->GetTranslate();
					ImGui::InputFloat3("VertexModel", &positionOBJ.x);
					ImGui::SliderFloat3("SliderVertexModel", &positionOBJ.x, -5.0f, 5.0f);
					objects[1]->SetTranslate(positionOBJ);


					rotationOBJ = objects[1]->GetRotate();
					ImGui::InputFloat3("RotateModel", &rotationOBJ.x);
					ImGui::SliderFloat3("SliderRotateModel", &rotationOBJ.x, -10.0f, 10.0f);

					objects[1]->SetRotate(rotationOBJ);


					sizeOBJ = objects[1]->GetScale();
					ImGui::InputFloat3("ScaleModel", &sizeOBJ.x);
					ImGui::SliderFloat3("SliderScaleModel", &sizeOBJ.x, 0.5f, 5.0f);

					objects[1]->SetScale(sizeOBJ);

					//ImGui::InputFloat3("MaterialModel", *);
					//ImGui::SliderFloat3("SliderMaterialModel", *inputMaterialModel, 0.0f, 1.0f);
					//ImGui::Checkbox("ModelTexture", &textureChange2);
				}
				ImGui::TreePop();
			}



			//if (ImGui::TreeNode("light")) {

			//	ImGui::InputFloat4("Materiallight", *inputMateriallight);
			//	ImGui::SliderFloat4("SliderMateriallight", *inputMateriallight, 0.0f, 1.0f);

			//	ImGui::InputFloat3("Vertexlight", *inputDirectionLight);
			//	ImGui::SliderFloat3("SliderVertexlight", *inputDirectionLight, -1.0f, 1.0f);


			//	ImGui::InputFloat("intensity", intensity);

			//	ImGui::TreePop();
			//}
			for (Sprite* sprite : sprites) {
				if (ImGui::TreeNode("Sprite")) {
					ImGui::Checkbox("IsSprite", &IsSprite);

					if (IsSprite) {

						position = sprite->GetPosition();
						ImGui::InputFloat("SpriteX", &position.x);
						ImGui::SliderFloat("SliderSpriteX", &position.x, 0.0f, 1000.0f);
						ImGui::InputFloat("SpriteY", &position.y);
						ImGui::SliderFloat("SliderSpriteY", &position.y, 0.0f, 600.0f);
						sprite->SetPosition(position);

						ImGui::DragFloat2("UVTranlate", &position.x, 0.01f, -10.0f, 10.0f);				
												
						size = sprite->GetSize();
						ImGui::DragFloat2("UVScale", &size.x, 0.1f);
						sprite->SetSize(size);

						rotation = sprite->GetRotate();
						ImGui::SliderAngle("UVRotate", &rotation);	
						sprite->SetRotate(rotation);	
		

						color = sprite->GetColor();
						ImGui::SliderFloat4("color", &color.x, 0.0f, 1.0f);
						sprite->SetColor(color);

					}
					ImGui::TreePop();
				}
			}
			//ImGuiの内部コマンド
			ImGui::Render();

			dxCommon->ProDraw();

			//モデル
			
			object3dCommon->Command();
			
			for (Object3d* object3d : objects) {
				object3d->Draw();
			}

			//UI
			spriteCommon->Command();

			for (Sprite* sprite : sprites) {
				sprite->Draw();
			}
			dxCommon->PostDraw();
		}
	}



	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	delete input_;
	winApp_->Finalize();

	delete winApp_;	
	winApp_ = nullptr;

	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();

	delete dxCommon;

	delete spriteCommon;
	for (Sprite* sprite : sprites) {
		delete sprite;
	}

	delete object3dCommon;
	for (Object3d* object3d : objects) {
		delete object3d;
	}

	delete modelCommon;
	//delete model;


	return 0;
}