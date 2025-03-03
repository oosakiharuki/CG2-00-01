#include "Framework.h"

void Framework::Initialize() {

	winApp_ = new WinApp();
	winApp_->Initialize();


	input_ = new Input();
	//GetHInstance()GetHwnd()を入れず直接winAppのクラスのものを使える
	input_->Initialize(winApp_);


	//ShowWindow(hwnd, SW_SHOW);


	dxCommon = new DirectXCommon();
	dxCommon->SetWinApp(winApp_);
	dxCommon->Initialize();

	srvManager = new SrvManager();
	srvManager->Initialize(dxCommon);

	ImGuiManager::GetInstance()->Initialize(winApp_, dxCommon, srvManager);

	TextureManager::GetInstance()->Initialize(dxCommon, srvManager);
	//TextureManager::GetInstance()->LoadTexture("resource/monsterBall.png");

	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon);


	object3dCommon = new Object3dCommon();
	object3dCommon->Initialize(dxCommon);


	modelCommon = new ModelCommon();
	modelCommon->Initialize(dxCommon);
	ModelManager::GetInstance()->Initialize(dxCommon);


	particleCommon = new ParticleCommon();
	particleCommon->Initialize(dxCommon);
	ParticleManager::GetInstance()->Initialize(dxCommon, srvManager);

}

void Framework::Update() {
	if (winApp_->ProcessMessage()) {
		isRequst = true;
	}
	else {
		//ゲームの処理
		input_->Update();
	}
}

void Framework::Finalize() {
	//旧WinApp
	D3DResourceLeakChecker leakCheck;

	delete input_;

	winApp_->Finalize();
	delete winApp_;
	winApp_ = nullptr;

	TextureManager::GetInstance()->Finalize();
	ModelManager::GetInstance()->Finalize();
	ParticleManager::GetInstance()->Finalize();

	dxCommon->Finalize();
	delete dxCommon;
	dxCommon = nullptr;

	delete srvManager;


	delete spriteCommon;
	delete object3dCommon;
	delete modelCommon;
	delete particleCommon;
}


void Framework::Run() {

	//ゲーム初期化
	Initialize();

	//ウィンドウの×ボタンが押されるまでループ
	while (true) {
		//毎フレーム更新
		Update();

		//終了リクエスト
		if (IsEndRequst()) {
			break;
		}
		//描画処理
		Draw();
	}

	//ゲーム処理
	Finalize();
}

