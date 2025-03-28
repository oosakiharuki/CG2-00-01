#pragma once
#include "WorldTransform.h"
#include "Object3d.h"
#include "MyMath.h"


class TestClass {
public:

	TestClass();
	~TestClass();

	void Init();
	void Update();
	void Draw();

private:
	WorldTransform worldTransform_;
	Object3d* object_ = nullptr;
	Camera* camera_ = nullptr;
	bool damage = false;
};
