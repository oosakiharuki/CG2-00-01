#include "GameManager.h"

GameManager::GameManager(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) {
	sceneArr_[Title] = new TitleScene();

	prevSceneNo_ = 0;
	currentSceneNo_ = Title;

	spriteCommon_ = spriteCommon;
	objCommon_ = objCommon;
	input_ = input;
}

GameManager::~GameManager() {
	for (uint32_t i = 0; i < SceneNum; ++i) {
		delete sceneArr_[i];
	}
}

void GameManager::SceneChange(int prev, int current) {

	delete sceneArr_[prev];
	sceneArr_[prev] = nullptr;

	//scene_ = current;
	switch (current)
	{
	case Title:
		sceneArr_[current] = new TitleScene();
		break;
	case GAME:
		sceneArr_[current] = new GameScene();
		break;
	case Clear:
		sceneArr_[current] = new ClearScene();
		break;
	case GameOver:
		sceneArr_[current] = new GameOverScene();
		break;
	}
}
void GameManager::Initialize() {
	sceneArr_[currentSceneNo_]->Initialize(spriteCommon_, objCommon_, input_);
}

void GameManager::Update() {

	prevSceneNo_ = currentSceneNo_;
	currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

	if (prevSceneNo_ != currentSceneNo_) {
		SceneChange(prevSceneNo_,currentSceneNo_);
		sceneArr_[currentSceneNo_]->Initialize(spriteCommon_, objCommon_, input_);
	}

	sceneArr_[currentSceneNo_]->Update();
}

void GameManager::Draw() {
	sceneArr_[currentSceneNo_]->Draw();
}