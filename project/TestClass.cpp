#include "TestClass.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Input.h"

using namespace MyMath;

TestClass::TestClass(){}

TestClass::~TestClass() {
	delete object_;
}

void TestClass::Init() {
	worldTransform_.Initialize();

	object_ = new Object3d();
	object_->Initialize();
	object_->SetModelFile("multiMaterial");
	
}

void TestClass::Update() {

	if (Input::GetInstance()->PushKey(DIK_S)) {
		damage = true;
	}
	else {
		damage = false;
	}

	ImGui::Begin("TestModel");

	ImGui::InputFloat3("VertexModel", &worldTransform_.translation_.x);
	ImGui::SliderFloat3("SliderVertexModel", &worldTransform_.translation_.x, -5.0f, 5.0f);

	ImGui::InputFloat3("RotateModel", &worldTransform_.rotation_.x);
	ImGui::SliderFloat3("SliderRotateModel", &worldTransform_.rotation_.x, -10.0f, 10.0f);

	ImGui::InputFloat3("ScaleModel", &worldTransform_.scale_.x);
	ImGui::SliderFloat3("SliderScaleModel", &worldTransform_.scale_.x, 0.5f, 5.0f);

	ImGui::End();

	object_->LightSwitch(damage);

	worldTransform_.UpdateMatrix();
}


void TestClass::Draw() {
	if (damage) {
		object_->Draw(worldTransform_);
	}
	else {
		object_->Draw(worldTransform_);
	}
}
