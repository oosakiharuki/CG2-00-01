#include "MyGame.h"

void MyGame::Initialize() {

	Framework::Initialize();

	gameScene = new GameScene();
	gameScene->Initialize();
}

void MyGame::Update() {

	Framework::Update();

	gameScene->Update();
}

void MyGame::Draw() {
	//描画開始
	DirectXCommon::GetInstance()->PreDraw();

	gameScene->Draw();

#ifdef  USE_IMGUI
	//ImGui描画処理
	ImGuiManager::GetInstance()->Draw();
#endif //  USE_IMGUI

	//描画終了
	DirectXCommon::GetInstance()->PostDraw();

}


void MyGame::Finalize() {
	gameScene->Finalize();
	delete gameScene;

	Framework::Finalize();
}
