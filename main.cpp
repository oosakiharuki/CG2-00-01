//#include<Windows.h>//winapp
#include <cstdint>
//#include <string>
#include<format>

#include<cassert>

#include <dxgidebug.h>

//#include <dxcapi.h>

#include <fstream>
#include <sstream>



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

using namespace Logger;
using namespace StringUtility;

//std::string str0{ "STRING!!!" };
//
//std::string str1{ std::to_string(10) };


struct Sphere {
	Vector3 center;
	float radius;
};


struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

Vector3 Normalize(const Vector3& v) {
	Vector3 result;
	result.x = v.x / (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	result.y = v.y / (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	result.z = v.z / (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return result;
}

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



//model
struct MaterialData {
	std::string textureFilePath;
};

MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	MaterialData materialData;
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	//ファイルを開く
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;

			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
};


struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};

ModelData LoadObjFile(const std::string& directoryPath,const std::string& filename) {
	ModelData modelData;
	
	//VertexData
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;	
	//ファイルから読んだ1行を格納する
	std::string line;
	//ファイルを読み取る
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	//構築
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);		
		s >> identifier; //先頭の義別子 (v ,vt, vn, f) を読み取る

		//modeldataの建築
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;//左から順に消費 = 飛ばしたり、もう一度使うことはできない	
			position.s = 1.0f;

			//反転
			position.x *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;

			//原点変更
			texcoord.y = 1.0f - texcoord.y;

			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			
			//反転
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {		
			VertexData triangle[3];
			
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v,index,'/'); //  "/"でインデックスを区切る
					elementIndices[element] = std::stoi(index);
					
				}
				
				
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//VertexData vertex = { position,texcoord,normal };
				//modelData.vertices.push_back(vertex);
				
				triangle[faceVertex] = { position,texcoord,normal };

			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;

			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);

		}
	}

	return modelData;
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


	TextureManager::GetInstance()->LoadTexture("resource/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resource/monsterBall.png");



	//////textureを読んで転送
	//uint32_t texture1  = TextureManager::GetInstance()->GetTextureIndexByFilePath("resource/uvChecker.png");

	////textureを読んで転送
	//uint32_t texture2 = TextureManager::GetInstance()->GetTextureIndexByFilePath("resource/monsterBall.png");//モンスターボール


	std::vector<Sprite*> sprites;

	for (uint32_t i = 0; i < 5; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteCommon, "resource/uvChecker.png");

		Vector2 position[5] = {};
		position[i].x += i * 200.0f;
		sprite->SetPosition(position[i]);
		
		sprites.push_back(sprite);
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
	
	ModelData modelData = LoadObjFile("resource", "teapot.obj");

	//static int modelChange = 0;



	//まだ	
	//ModelData modelData = LoadObjFile("resource", "multiMaterial.obj");
	//ModelData modelData = LoadObjFile("resource", "stanford-bunny.obj");


	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceModel = dxCommon->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewModel{};
	vertexBufferViewModel.BufferLocation = vertexResourceModel->GetGPUVirtualAddress();
	vertexBufferViewModel.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewModel.StrideInBytes = sizeof(VertexData);

	Microsoft::WRL::ComPtr<ID3D12Resource> wvpResourceModel = dxCommon->CreateBufferResource(sizeof(TransformationMatrix));
	TransformationMatrix* wvpDataModel = nullptr;
	wvpResourceModel->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataModel));
	wvpDataModel->World = MakeIdentity4x4();

	VertexData* vertexDataModel = nullptr;
	vertexResourceModel->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataModel));
	std::memcpy(vertexDataModel, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	//textureを読んで転送
	//mipImages2 = LoadTexture(modelData.material.textureFilePath);


	////三角用マテリアル
	////マテリアル用のリソース
	//Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = dxCommon->CreateBufferResource(sizeof(Vector4));
	////マテリアルにデータを書き込む
	//Vector4* materialData = nullptr;
	////書き込むためのアドレス
	//materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	////色の設定
	//*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);




	//球体用マテリアル
	//マテリアル用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSphere = dxCommon->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	Material* materialDataSphere = nullptr;
	//書き込むためのアドレス
	materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere));
	//色の設定
	materialDataSphere->color =  Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSphere->enableLighting = true;
	materialDataSphere->uvTransform = MakeIdentity4x4();



	//Model用マテリアル
	//マテリアル用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceModel = dxCommon->CreateBufferResource(sizeof(Material));
	//マテリアルにデータを書き込む
	Material* materialDataModel = nullptr;
	//書き込むためのアドレス
	materialResourceModel->Map(0, nullptr, reinterpret_cast<void**>(&materialDataModel));
	//色の設定
	materialDataModel->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataModel->enableLighting = true;
	materialDataModel->uvTransform = MakeIdentity4x4();



	//ライト用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightSphereResource = dxCommon->CreateBufferResource(sizeof(DirectionalLight));
	//マテリアルにデータを書き込む
	DirectionalLight* directionalLightSphereData = nullptr;
	//書き込むためのアドレス
	directionalLightSphereResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightSphereData));
	//色の設定
	directionalLightSphereData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightSphereData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightSphereData->intensity = 1.0f;






	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };
	Transform cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}, {0.0f,0.0f,-10.5f} };


	Transform transformSphere{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };

	Transform transformModel{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };

	Transform transformL{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f} };





	//float *inputMaterial[3] = { &materialData->x,&materialData->y,&materialData->z };
	//float* inputTransform[3] = { &transform.translate.x,&transform.translate.y,&transform.translate.z };
	//float* inputRotate[3] = { &transform.rotate.x,&transform.rotate.y,&transform.rotate.z };
	//float* inputScale[3] = { &transform.scale.x,&transform.scale.y,&transform.scale.z };


	float* inputMaterialSphere[3] = { &materialDataSphere->color.x,&materialDataSphere->color.y,&materialDataSphere->color.z };
	float* inputTransformSphere[3] = { &transformSphere.translate.x,&transformSphere.translate.y,&transformSphere.translate.z };
	float* inputRotateSphere[3] = { &transformSphere.rotate.x,&transformSphere.rotate.y,&transformSphere.rotate.z };
	float* inputScaleSphere[3] = { &transformSphere.scale.x,&transformSphere.scale.y,&transformSphere.scale.z };
	bool textureChange = false;

	float* inputMaterialModel[3] = { &materialDataModel->color.x,&materialDataModel->color.y,&materialDataModel->color.z };
	float* inputTransformModel[3] = { &transformModel.translate.x,&transformModel.translate.y,&transformModel.translate.z };
	float* inputRotateModel[3] = { &transformModel.rotate.x,&transformModel.rotate.y,&transformModel.rotate.z };
	float* inputScaleModel[3] = { &transformModel.scale.x,&transformModel.scale.y,&transformModel.scale.z };
	bool textureChange2 = false;


	float* inputMateriallight[3] = { &directionalLightSphereData->color.x,&directionalLightSphereData->color.y,&directionalLightSphereData->color.z };
	float* inputDirectionLight[3] = { &directionalLightSphereData->direction.x,&directionalLightSphereData->direction.y,&directionalLightSphereData->direction.z };
	float* intensity = &directionalLightSphereData->intensity;

	//描画させるもの
	bool IsSphere = true;
	bool IsModel = true;
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


				position = sprite->GetPosition();
				position.x += 0.1f;
				position.y += 0.1f;
				sprite->SetPosition(position);

				rotation = sprite->GetRotate();
				rotation += 0.01f;
				sprite->SetRotate(rotation);

				color = sprite->GetColor();
				color.x += 0.01f;
				if (color.x > 1.0f) {
					color.x -= 1.0f;
				}
				sprite->SetColor(color);

				size = sprite->GetSize();
				size.x += 0.1f;
				size.y += 0.1f;
				sprite->SetSize(size);
			}


			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			//transform.rotate.y += 0.03f;

			Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
			Matrix4x4 viewMatrix = Inverse(cameraMatrix);
			Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);
			//Matrix4x4 projectionMatrix = MakeOrthographicMatrix((float)scissorRect.left, (float)scissorRect.top, (float)scissorRect.right, (float)scissorRect.bottom, 0.1f, 100.0f);
			Matrix4x4 WorldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

			//三角
			//*wvpData = WorldViewProjectionMatrix;


			//球体

			Matrix4x4 worldMatrixSphere = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
			Matrix4x4 WorldViewProjectionMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));

			wvpDataSphere->World = worldMatrixSphere;
			wvpDataSphere->WVP = WorldViewProjectionMatrixSphere;
					
			DrawSphere(vertexDataSphere);
		
			
			//モデル
			Matrix4x4 worldMatrixModel = MakeAffineMatrix(transformModel.scale, transformModel.rotate, transformModel.translate);
			Matrix4x4 WorldViewProjectionMatrixModel = Multiply(worldMatrixModel, Multiply(viewMatrix, projectionMatrix));
			
			wvpDataModel->World = worldMatrixModel;
			wvpDataModel->WVP = WorldViewProjectionMatrixModel;

			directionalLightSphereData->direction = Normalize(directionalLightSphereData->direction);

		

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

			//if (ImGui::TreeNode("Model")) {
			//	ImGui::Checkbox("IsModel", &IsModel);
			//	//ImGui::Combo("combo", &modelChange, "plane\0aixe\0teapot\0\0");
			//	//ImGui::RadioButton("plane", &modelChange, 0); ImGui::SameLine();
			//	//ImGui::RadioButton("axis", &modelChange, 1); ImGui::SameLine();
			//	//ImGui::RadioButton("teapot", &modelChange, 2);
			//	if (IsModel) {
			//		ImGui::InputFloat3("MaterialModel", *inputMaterialModel);
			//		ImGui::SliderFloat3("SliderMaterialModel", *inputMaterialModel, 0.0f, 1.0f);

			//		ImGui::InputFloat3("VertexModel", *inputTransformModel);
			//		ImGui::SliderFloat3("SliderVertexModel", *inputTransformModel, -5.0f, 5.0f);

			//		ImGui::InputFloat3("RotateModel", *inputRotateModel);
			//		ImGui::SliderFloat3("SliderRotateModel", *inputRotateModel, -10.0f, 10.0f);

			//		ImGui::InputFloat3("ScaleModel", *inputScaleModel);
			//		ImGui::SliderFloat3("SliderScaleModel", *inputScaleModel, 0.5f, 5.0f);

			//		ImGui::Checkbox("ModelTexture", &textureChange2);			
			//	}
			//	ImGui::TreePop();
			//}


			//if (ImGui::TreeNode("light")) {

			//	ImGui::InputFloat4("Materiallight", *inputMateriallight);
			//	ImGui::SliderFloat4("SliderMateriallight", *inputMateriallight, 0.0f, 1.0f);

			//	ImGui::InputFloat3("Vertexlight", *inputDirectionLight);
			//	ImGui::SliderFloat3("SliderVertexlight", *inputDirectionLight, -1.0f, 1.0f);


			//	ImGui::InputFloat("intensity", intensity);

			//	ImGui::TreePop();
			//}

			if (ImGui::TreeNode("Sprite")) {
				ImGui::Checkbox("IsSprite", &IsSprite);

				if (IsSprite) {
					ImGui::InputFloat("SpriteX", &position.x);
					ImGui::SliderFloat("SliderSpriteX", &position.x, 0.0f, 1000.0f);

					ImGui::InputFloat("SpriteY", &position.y);
					ImGui::SliderFloat("SliderSpriteY", &position.y, 0.0f, 600.0f);

					ImGui::DragFloat2("UVTranlate", &position.x, 0.01f, -10.0f, 10.0f);
					ImGui::DragFloat2("UVScale", &size.x, 0.01f, -10.0f, 10.0f);
					ImGui::SliderAngle("UVRotate", &rotation);
				}
				ImGui::TreePop();
			}

			//ImGuiの内部コマンド
			ImGui::Render();

			dxCommon->ProDraw();

			spriteCommon->Command();
			
			//三角
			//dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress()); //rootParameterの配列の0番目 [0]
			//dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
			//dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);	
			//dxCommon->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			//dxCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);


			//球体

			//if (IsSphere) {
			//	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSphere);
			//	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSphere->GetGPUVirtualAddress()); //rootParameterの配列の0番目 [0]
			//	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResourceSphere->GetGPUVirtualAddress());

			//	if (textureChange) {
			//		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU2);
			//	}
			//	else {
			//		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
			//	}
			//	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightSphereResource->GetGPUVirtualAddress());
			//	dxCommon->GetCommandList()->ClearDepthStencilView(dxCommon->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			//	dxCommon->GetCommandList()->DrawInstanced(SphereVertexNum, 1, 0, 0);

			//}

			////モデル
			//if (IsModel) {
			//	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewModel);
			//	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceModel->GetGPUVirtualAddress()); //rootParameterの配列の0番目 [0]
			//	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResourceModel->GetGPUVirtualAddress());
			//	if (textureChange2) {
			//		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU2);
			//	}
			//	else {
			//		dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);
			//	}
			//	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightSphereResource->GetGPUVirtualAddress());
			//	dxCommon->GetCommandList()->ClearDepthStencilView(dxCommon->GetDsvHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
			//	dxCommon->GetCommandList()->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

			//}

			//UI
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

	delete dxCommon;

	TextureManager::GetInstance()->Finalize();

	delete spriteCommon;
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	return 0;
}