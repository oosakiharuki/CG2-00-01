#include "Input.h"
#include "DirectXCommon.h"
#include "D3DResorceLeakChecker.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#include "Model.h"
#include "ModelManager.h"
#include "MyMath.h"

using namespace MyMath;


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

	std::vector <Object3d*> objects;


	Vector3 position[2] = {};

	for (uint32_t i = 0; i < 2; ++i) {

		Object3d* object3d = new Object3d();
	
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
			
			
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();


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



			//開発用UIの処理
			//ImGui::ShowDemoWindow();

			//ここにテキストを入れられる
			ImGui::Text("ImGuiText");

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
	
	dxCommon->Finalize();
	delete dxCommon;
	dxCommon = nullptr;

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