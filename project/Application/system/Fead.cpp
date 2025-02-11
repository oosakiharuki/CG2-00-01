#include "Fead.h"

Fead::~Fead() {
	delete sprite;
}

void Fead::Initialize(SpriteCommon* spriteCommon, std::string spriteFile) {
	spriteCommon_ = spriteCommon;
	
	sprite = new Sprite();
	spriteFile_ = spriteFile;
	sprite->Initialize(spriteCommon, spriteFile_);

	color = { 1.0f,1.0f,1.0f,1.0f };
	sprite->SetColor(color);
}

void Fead::Update() {

	sprite->Update();

	switch (status)
	{
	case Status::FeadIn:
		if (startFead) {
			sprite->SetSprite(spriteFile_);
			color = sprite->GetColor();
			color.s += 0.025f;
			sprite->SetColor(color);

			if (sprite->GetColor().s > 1.0f) {
				color.s = 1.0f;
				isFinish = true;
				status = Status::FeadOut;
			}
		}
		break;
	case Status::FeadOut:
		sprite->Update();
		color = sprite->GetColor();
		color.s -= 0.025f;
		sprite->SetColor(color);

		if (sprite->GetColor().s < 0.0f) {
			color.s = 0.0f;
			sprite->SetColor(color);
			status = Status::FeadIn;
		}
		break;
	}	

}

void Fead::Draw() {
	sprite->Draw();
}

void Fead::StartFead() {
	startFead = true;
}

