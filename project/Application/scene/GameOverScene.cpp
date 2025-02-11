#include "GameOverScene.h"

GameOverScene::~GameOverScene() {
	delete fead_;
	delete sprite_;
	delete camera;
}

void GameOverScene::Initialize(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) {
	this->spriteCommon_ = spriteCommon;
	this->object3dCommon_ = objCommon;
	this->input_ = input;

	fead_ = new Fead();
	fead_->Initialize(spriteCommon_, "resource/Fead.png");

	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon_, "resource/LozeScene.png");

	camera = new Camera();
	Vector3 cameraRotate = { 0.0f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,0.0f,-15.0f };

	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);
	object3dCommon_->SetDefaultCamera(camera);

	audioRetry = new Audio();
	audioRetry->Initialize("resource/select.wav");

	audioBack = new Audio();
	audioBack->Initialize("resource/back.wav");
}

void GameOverScene::Update() {
	camera->Update();
	input_->Update();
	fead_->Update();
	sprite_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		fead_->StartFead();
		reTry = true;
		audioHandle1  -=1;
	}
	if (input_->TriggerKey(DIK_ESCAPE)) {
		fead_->StartFead();
		audioHandle2  -=1;
	}

	if (fead_->SceneChange() && reTry) {
		sceneNo = SCENE::GAME;
	}
	else if(fead_->SceneChange()){
		sceneNo = SCENE::Title;
	}

	if (audioHandle1 < 0) {
		audioRetry->SoundPlayWave(0.05f);
		audioHandle1 += 10;
	}
	
	if (audioHandle2 < 0) {
		audioBack->SoundPlayWave(0.05f);
		audioHandle2 += 10;
	}

}

void GameOverScene::Draw() {
	//object
	object3dCommon_->Command();

	spriteCommon_->Command();
	sprite_->Draw();
	fead_->Draw();
}
