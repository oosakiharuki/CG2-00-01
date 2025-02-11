#pragma once
#include "Input.h"
//#include "WinApp.h"
#include "DirectXCommon.h"
#include "D3DResorceLeakChecker.h"
#include "GameManager.h"

#include "IScene.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#include "Model.h"
#include "ModelManager.h"
#include "SrvManager.h"
#include "Fead.h"

#include "Audio.h"

class GameScene : public IScene{
public:
	~GameScene() override;
	void Initialize(SpriteCommon* spriteCommon, Object3dCommon* objCommon, Input* input) override;
	void Update() override;
	void Draw() override;
	//
	//bool IsCollision(const AABB& aabb1, const AABB& aabb2);
	//void CheckAllCollisions();

	//void EnemyBorn(Vector3 position);
	//void LoadEnemyPopData();
	//void UpdateEnemyPop();

	void PlayAudio();

private:
	Input* input_ = nullptr;
	Object3dCommon* object3dCommon_ = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;

	Camera* camera = nullptr;
	//int shakeX;
	//int shakeZ;
	
	//Player* player = nullptr;
	//std::list<Enemy*> enemies;
	//std::stringstream enemyPopCommands;
	//bool WaitFlag = false;
	//int32_t waitTimer;
	//bool isBornFinish = false;
	//float speed = 0.05f;

	Fead* fead_ = nullptr;
	//Skydorm* skydorm_ = nullptr;

	//Wall* wall_ = nullptr;

	bool isClear_ = false;
	bool isGameOver_ = false;

	//Sprite* UI = nullptr;

	Audio* BGM = nullptr;
	int BGMHandle = -1;
	float rePlay = 61.0f;

	Audio* audio_ = nullptr;
	int audioHandle = 0;
};