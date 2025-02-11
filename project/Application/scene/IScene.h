#pragma once

#include "SpriteCommon.h"
#include "Object3dCommon.h"
#include "Input.h"
#include "Camera.h"

enum SCENE{
	Title,
	GAME,
	Clear,
	GameOver,
};


class IScene{
protected:
	static int sceneNo;

public:
	virtual void Initialize(SpriteCommon* spriteCommon,Object3dCommon* objCommon,Input* input) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	int GetSceneNo();
};