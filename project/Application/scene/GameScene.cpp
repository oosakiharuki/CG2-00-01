#include "GameScene.h"
#include <cassert>
#include <fstream>

GameScene::~GameScene() {
	delete camera;
	delete fead_;
	delete BGM;
}

void GameScene::Initialize(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) {
	
	this->input_ = input;
	this->object3dCommon_ = objCommon;
	this->spriteCommon_ = spriteCommon;

	camera = new Camera();
	//Vector3 cameraRotate = { 0.25f,0.0f,0.0f };
	//Vector3 cameraTranslate = { 0.0f,7.0f,-20.0f };
	//camera->SetRotate(cameraRotate);
	//camera->SetTranslate(cameraTranslate);
	object3dCommon_->SetDefaultCamera(camera);


	fead_ = new Fead();
	fead_->Initialize(spriteCommon_,"resource/Fead.png");

	isClear_ = false;
	isGameOver_ = false;

	BGM = new Audio();
	BGM->Initialize("resource/BGM.wav");

	audio_ = new Audio();
	audio_->Initialize("resource/damageE.wav");
}

void GameScene::Update() {
	input_->Update();
	camera->Update();
	fead_->Update();
	//SceneChange

	if (input_->TriggerKey(DIK_1)) {
		isGameOver_ = true;
		fead_->StartFead();
	}
	else if (input_->TriggerKey(DIK_2)) {
		isClear_ = true;
		fead_->ChangeSpriteFile("resource/FeadWin.png");
		fead_->StartFead();	
	}

	if (fead_->SceneChange() && isGameOver_) {
		sceneNo = SCENE::GameOver;
	}
	else if (fead_->SceneChange() && isClear_) {
		sceneNo = SCENE::Clear;
	}

	PlayAudio();

#ifdef _DEBUG
	ImGui::Text("Push keyBorad 1 : gameover");
	ImGui::Text("Push keyBorad 2 : gameClear");

#endif

}

void GameScene::Draw() {

	//backScreen
	spriteCommon_->Command();


	//object
	object3dCommon_->Command();

	//UI
	spriteCommon_->Command();

	fead_->Draw();

}


void GameScene::PlayAudio() {

	rePlay -= 1.0f / 60.0f;
	if (rePlay < 0.0f) {
		BGMHandle = -1;
		rePlay = 61.0f;
	}

	if (BGMHandle < 0) {
		BGM->SoundPlayWave(0.02f);
		BGMHandle++;
	}

	if (audioHandle < 0) {
		audio_->SoundPlayWave(0.05f);
		audioHandle++;
	}
}
