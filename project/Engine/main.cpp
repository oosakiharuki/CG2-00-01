#include "GameManager.h"

using namespace MyMath;


//Windowsアプリのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//旧WinApp
	D3DResourceLeakChecker leakCheck;

	WinApp* winApp_ = nullptr;

	winApp_ = new WinApp();
	winApp_->Initialize();

	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->SetWinApp(winApp_);
	dxCommon->Initialize();

	Input* input_;
	input_ = new Input();
	input_->Initialize(winApp_);

	SrvManager* srvManager = nullptr;
	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	TextureManager::GetInstance()->Initialize(dxCommon,srvManager);

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);

	ModelManager::GetInstance()->Initialize(dxCommon);

	Object3dCommon* object3dCommon = nullptr;
	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);

	ModelCommon* modelCommon = nullptr;
	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);


	GameManager* gameScene = nullptr;
	gameScene = new GameManager(spriteCommon, object3dCommon, input_);
	gameScene->Initialize();


	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		if (winApp_->ProcessMessage()) {
			break;//whileを抜ける
		}
		else {
			//ゲームの処理
			
			
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();


			gameScene->Update();

#ifdef _DEBUG
			//開発用UIの処理
			//ImGui::ShowDemoWindow();
#endif
			//ImGuiの内部コマンド
			ImGui::Render();


			dxCommon->PreDraw();

			gameScene->Draw();

			dxCommon->PostDraw();
		}
	}

	delete input_;
	
	winApp_->Finalize();
	delete winApp_;	
	winApp_ = nullptr;
	
	dxCommon->Finalize();
	delete dxCommon;
	dxCommon = nullptr;

	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();

	delete srvManager;
	delete spriteCommon;
	delete object3dCommon;
	delete modelCommon;

	delete gameScene;

	return 0;
}