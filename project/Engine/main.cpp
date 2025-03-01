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
#include "SrvManager.h"
#include "particle.h" 
#include "ParticleCommon.h"
#include "ParticleManager.h"
#include "ImGuiManager.h"
#include "Audio.h"

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

	SrvManager* srvManager = nullptr;
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	ImGuiManager::GetInstance()->Initialize(winApp_,dxCommon,srvManager);

	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);
	//TextureManager::GetInstance()->LoadTexture("resource/monsterBall.png");

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	std::vector<Sprite*> sprites;

	for (uint32_t i = 0; i < 1; ++i) {
		Sprite* sprite = new Sprite();
		//if (i == 1 || i == 3) {
			sprite->Initialize(spriteCommon, "resource/monsterBall.png");
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

	Camera* camera = new Camera();
	//Vector3 cameraRotate = { 1.4f,0.0f,0.0f };
	//Vector3 cameraTranslate = { 0.0f,30.0f,-8.0f };
	Vector3 cameraRotate = { 0.0f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,0.0f,-15.0f };
	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);
	object3dCommon->SetDefaultCamera(camera);

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

		//position[i].x += i * 3.0f;
		//object3d->SetTranslate(position[i]);

		objects.push_back(object3d);
	}

	ParticleCommon* particleCommon = nullptr; 
	particleCommon = new ParticleCommon();
	particleCommon->Initialize(dxCommon);
	particleCommon->SetDefaultCamera(camera);
	
	ParticleManager::GetInstance()->Initialize(dxCommon,srvManager);
	ParticleManager::GetInstance()->CreateParticleGroup("particle01", "resource/circle.png");
	ParticleManager::GetInstance()->CreateParticleGroup("particle02","resource/uvChecker.png");

	Particle* particle = new Particle();
	particle->Initialize(particleCommon,"particle02");

	Particle* particle2 = new Particle();
	particle2->Initialize(particleCommon, "particle01");

	//Audio* audio = new Audio();
	//audio->Initialize("resource/audio01.wav");
	//int audioHandle = 0;

	Audio* audio2 = new Audio();
	audio2->Initialize("resource/audio01.wav");
	int audioHandle2 = 0;

	

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


			ImGuiManager::GetInstance()->Begin();


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
			//rotationOBJ2.z += 0.1f;			

			if (input_->PushKey(DIK_D)) {
				positionOBJ.x += 0.1f;
			}

			if (input_->PushKey(DIK_A)) {
				positionOBJ.x -= 0.1f;
			}

			objects[1]->SetTranslate(positionOBJ);

			particle->Update();			
			particle2->Update();

			particle->SetTranslate(positionOBJ);
			particle->SetFrequency(0.5f);

			//Vector3 rotateP = particle->GetRotate();
			//rotateP.z = 0.1f;
			//particle->SetRotate(rotateP);
			
			Vector3 sizeP = { 1,1,1 };
			particle->SetScale(sizeP);

			camera->Update();
			
			
			//if (input_->TriggerKey(DIK_2)) {
			//	audioHandle = -1;
			//}

			//if (audioHandle < 0) {
			//	audio->SoundPlayWave(0.05f);
			//	audioHandle++;
			//}


			if (input_->TriggerKey(DIK_3)) {
				audioHandle2 = -1;
			}

			if (audioHandle2 < 0) {
				audio2->SoundPlayWave(0.05f);
				audioHandle2++;
			}



#ifdef  USE_IMGUI
			//ここにテキストを入れられる
			
			//開発用UIの処理
			ImGui::ShowDemoWindow();

			ImGui::Text("ImGuiText");

			//カメラ
			ImGui::SliderFloat3("cameraTranslate", &cameraTranslate.x, -30.0f, 30.0f);

			ImGui::SliderFloat("cameraRotateX", &cameraRotate.x, -10.0f, 10.0f);
			ImGui::SliderFloat("cameraRotateY", &cameraRotate.y, -10.0f, 10.0f);
			ImGui::SliderFloat("cameraRotateZ", &cameraRotate.z, -10.0f, 10.0f);
			camera->SetRotate(cameraRotate);
			camera->SetTranslate(cameraTranslate);


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
			

#endif //  USE_IMGUI

			ImGuiManager::GetInstance()->End();

			//描画開始
			dxCommon->PreDraw();
			srvManager->PreDraw();
			//モデル
			
			object3dCommon->Command();
			
			for (Object3d* object3d : objects) {
				object3d->Draw();
			}

			particleCommon->Command();

			particle->Draw();
			//particle2->Draw();

			//UI
			spriteCommon->Command();

			for (Sprite* sprite : sprites) {
				sprite->Draw();
			}		
			
			ImGuiManager::GetInstance()->Draw();

			dxCommon->PostDraw();

		}
	}

	delete input_;
	
	winApp_->Finalize();
	delete winApp_;	
	winApp_ = nullptr;

	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();

	dxCommon->Finalize();
	delete dxCommon;
	dxCommon = nullptr;

	delete srvManager;

	ImGuiManager::GetInstance()->Finalize();


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

	delete particle;
	delete particle2;
	delete particleCommon;

	//delete audio;
	delete audio2;

	return 0;
}