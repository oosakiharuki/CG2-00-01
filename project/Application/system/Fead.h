#pragma once
#include "IScene.h"
#include "Sprite.h"

enum class Status {
	FeadIn,
	FeadOut
};


class Fead  {
public:
	~Fead() ;
	void Initialize(SpriteCommon* spriteCommon,std::string spriteFile);
	void Update();
	void Draw();

	void StartFead();
	bool SceneChange() { return isFinish; }
	void ChangeSpriteFile(std::string spriteFile) { spriteFile_ = spriteFile; }

private:
	Sprite* sprite = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;

	std::string spriteFile_;
	Vector4 color;
	bool startFead = false;
	bool isFinish = false;
	Status status = Status::FeadOut;
};