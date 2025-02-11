#pragma once

#include <memory>
#include "IScene.h"
#include "GameScene.h"//geme
#include "TitleScene.h"//title
#include "ClearScene.h"//clear
#include "GameOverScene.h"//gameover


class GameManager {
public:
	GameManager(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input);
	~GameManager();

	void Initialize();
	void Update();
	void Draw();

private:
	
	void SceneChange(int prev,int current);//シーン入れ替え

	IScene* sceneArr_[4];
	uint32_t SceneNum = 4;


	int currentSceneNo_;//現在シーン
	int prevSceneNo_;//前シーン

	SpriteCommon* spriteCommon_;
	Object3dCommon* objCommon_;
	Input* input_;
};