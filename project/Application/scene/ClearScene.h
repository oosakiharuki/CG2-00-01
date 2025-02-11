#pragma once
#include "IScene.h"
#include "Input.h"
#include "Fead.h"
#include "Audio.h"

class ClearScene : public IScene {
public:
	~ClearScene() override;
	void Initialize(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) override;
	void Update() override;
	void Draw() override;
private:
	Input* input_ = nullptr;
	Object3dCommon* object3dCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Sprite* sprite_ = nullptr;
	Fead* fead_ = nullptr;
	Camera* camera = nullptr;

	Audio* audio_ = nullptr;
	int audioHandle = 0;
};