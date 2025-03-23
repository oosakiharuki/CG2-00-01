#include "GameScene.h"

void GameScene::Initialize() {


	for (uint32_t i = 0; i < 1; ++i) {
		Sprite* sprite = new Sprite();
		//if (i == 1 || i == 3) {
		sprite->Initialize("resource/monsterBall.png");
		//}
		//else {
		sprite->Initialize("resource/uvChecker.png");
		//}
		Vector2 position[5] = {};
		position[i].x += i * 200.0f;
		sprite->SetPosition(position[i]);

		sprites.push_back(sprite);
	}

	spriteUI = new Sprite();
	spriteUI->Initialize("resource/monsterBall.png");
	
	spriteUI->SetPosition({ 64,64 });
	spriteUI->SetSize({ 128,128 });


	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");


	camera = new Camera();
	//Vector3 cameraRotate = { 1.4f,0.0f,0.0f };
	//Vector3 cameraTranslate = { 0.0f,30.0f,-8.0f };
	cameraRotate = { 0.0f,0.0f,0.0f };
	cameraTranslate = { 0.0f,0.0f,-15.0f };
	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);

	Object3dCommon::GetInstance()->SetDefaultCamera(camera);
	ParticleCommon::GetInstance()->SetDefaultCamera(camera);

	Vector3 position[2] = {};

	for (uint32_t i = 0; i < 2; ++i) {

		Object3d* object3d = new Object3d();

		if (i == 0) {
			object3d->SetModelFile("plane.obj");
			object3d->Initialize();
		}
		else {
			object3d->SetModelFile("axis.obj");
			object3d->Initialize();
		}

		//position[i].x += i * 3.0f;
		//object3d->SetTranslate(position[i]);

		objects.push_back(object3d);
	}



	ParticleManager::GetInstance()->CreateParticleGroup("particle01", "resource/circle.png");
	ParticleManager::GetInstance()->CreateParticleGroup("particle02", "resource/uvChecker.png");

	particle = new Particle();
	particle->Initialize(ParticleCommon::GetInstance(), "particle02");

	particle2 = new Particle();
	particle2->Initialize(ParticleCommon::GetInstance(), "particle01");

	//Audio* audio = new Audio();
	//audio->Initialize("resource/audio01.wav");
	//int audioHandle = 0;

	audio2 = new Audio();
	audio2->Initialize("resource/audio01.wav");

}

void GameScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_0)) {
		OutputDebugStringA("Hit 0\n");
	}

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		OutputDebugStringA("Hit 1\n");
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = Title;
	}

	Vector2 position;
	float rotation;
	Vector4 color;
	Vector2 size;
	
	for (Sprite* sprite : sprites) {
		sprite->Update();


		position = sprite->GetPosition();
		//position.x += 0.1f;
		//position.y += 0.1f;
		sprite->SetPosition(position);

		rotation = sprite->GetRotate();
		//rotation += 0.01f;
		sprite->SetRotate(rotation);

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
	spriteUI->Update();


	Vector3 positionOBJ;
	Vector3 rotationOBJ;
	Vector3 rotationOBJ2;
	Vector3 sizeOBJ;

	Input::GetInstance()->GetJoyStickState(0, state);
	Input::GetInstance()->GetJoystickStatePrevious(0, preState);
	
	for (Object3d* object3d : objects) {
		object3d->Update();
		
		positionOBJ = object3d->GetTranslate();
		
		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
			!(preState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			positionOBJ.y += 0.5f;
		}

		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) &&
			(preState.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
			positionOBJ.y -= 0.5f;
		}		
		
		if (!(state.Gamepad.wButtons & XINPUT_GAMEPAD_X) &&
			(preState.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
			positionOBJ.z += 0.5f;
		}

		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
			!(preState.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			OutputDebugStringA("Hit A Botton\n");
		}

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

	if (Input::GetInstance()->PushKey(DIK_D)) {
		positionOBJ.x += 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_A)) {
		positionOBJ.x -= 0.1f;
	}
	
	float x = 0, z = 0;
	const float speed = 0.2f;

	if (Input::GetInstance()->GetJoyStickState(0, state)) {
		// 左スティックの入力
		x = static_cast<float>(state.Gamepad.sThumbLX) / 32768.0f; // -1.0f～1.0f
		z = static_cast<float>(state.Gamepad.sThumbLY) / 32768.0f; // -1.0f～1.0f

		// デッドゾーン処理
		const float deadZone = 0.2f; // スティックの感度調整
		if (abs(x) < deadZone) {
			x = 0.0f;
		}
		if (abs(z) < deadZone) {
			z = 0.0f;
		}
		positionOBJ.x += x * speed;
		positionOBJ.z += z * speed;

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


	if (Input::GetInstance()->TriggerKey(DIK_3)) {
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
}

void GameScene::Draw() {

	//スプライト描画処理(背景用)
	SpriteCommon::GetInstance()->Command();

	for (Sprite* sprite : sprites) {
		sprite->Draw();
	}

	//モデル描画処理
	Object3dCommon::GetInstance()->Command();

	for (Object3d* object3d : objects) {
		object3d->Draw();
	}

	//パーティクル描画処理
	ParticleCommon::GetInstance()->Command();

	particle->Draw();
	particle2->Draw();

	//スプライト描画処理(UI用)
	SpriteCommon::GetInstance()->Command();

	spriteUI->Draw();

}
void GameScene::Finalize() {

	delete camera;

	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	delete spriteUI;

	for (Object3d* object3d : objects) {
		delete object3d;
	}

	//delete model;

	delete particle;
	delete particle2;

	//delete audio;
	delete audio2;
}
