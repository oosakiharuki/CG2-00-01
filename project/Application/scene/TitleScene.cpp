#include "TitleScene.h"
#include "ModelManager.h"

TitleScene::~TitleScene() {
	delete fead_;
	delete camera;
	delete sprite_;
	delete audio_;
}

void TitleScene::Initialize(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) {
	this->spriteCommon_ = spriteCommon;
	this->object3dCommon_ = objCommon;
	this->input_ = input;

	fead_ = new Fead();
	fead_->Initialize(spriteCommon_, "resource/Fead.png");

	camera = new Camera();
	Vector3 cameraRotate = { 0.0f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,0.0f,-15.0f };

	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);
	object3dCommon_->SetDefaultCamera(camera);


	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon_,"resource/Title.png");

	audio_ = new Audio();
	audio_->Initialize("resource/select.wav");
}

void TitleScene::Update() {	
	camera->Update();
	input_->Update();
	sprite_->Update();
	fead_->Update();


	if (input_->TriggerKey(DIK_SPACE)) {
		fead_->StartFead();
		audioHandle -= 1;
	}
	if (audioHandle < 0) {
		audio_->SoundPlayWave(0.05f);
		audioHandle += 10;
	}

	if (fead_->SceneChange()) {
		sceneNo = SCENE::GAME;
	}
}

void TitleScene::Draw() {
	//object
	object3dCommon_->Command();

	//UI
	spriteCommon_->Command();
	sprite_->Draw();
	fead_->Draw();
}
