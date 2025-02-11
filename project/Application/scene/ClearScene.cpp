#include "ClearScene.h"

ClearScene::~ClearScene(){
	delete fead_;
	delete sprite_;
	delete camera;
}

void ClearScene::Initialize(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) {
	this->spriteCommon_ = spriteCommon;
	this->object3dCommon_ = objCommon;
	this->input_ = input;

	fead_ = new Fead();
	fead_->Initialize(spriteCommon_,"resource/FeadWin.png");

	sprite_ = new Sprite();
	sprite_->Initialize(spriteCommon_,"resource/winScene.png");

	camera = new Camera();
	Vector3 cameraRotate = { 0.0f,0.0f,0.0f };
	Vector3 cameraTranslate = { 0.0f,0.0f,-15.0f };

	camera->SetRotate(cameraRotate);
	camera->SetTranslate(cameraTranslate);
	object3dCommon_->SetDefaultCamera(camera);


	audio_ = new Audio();
	audio_->Initialize("resource/select.wav");
}

void ClearScene::Update() {
	camera->Update();
	input_->Update();
	sprite_->Update();
	fead_->Update();

	if (input_->TriggerKey(DIK_SPACE)) {
		fead_->ChangeSpriteFile("resource/Fead.png");
		fead_->StartFead();
		audioHandle -= 1;
	}

	if (fead_->SceneChange()) {
		sceneNo = SCENE::Title;
	}

	if (audioHandle < 0) {
		audio_->SoundPlayWave(0.05f);
		audioHandle += 10;
	}
}

void ClearScene::Draw() {	
	//object
	object3dCommon_->Command();

	spriteCommon_->Command();
	sprite_->Draw();
	fead_->Draw();
}
