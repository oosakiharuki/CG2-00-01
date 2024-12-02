#include "Sprite.h"
#include "SpriteCommon.h"
//#include "externals/imgui/imgui.h"

using namespace MyMath;

void Sprite::Initialize(SpriteCommon* spriteCommon) {
	this->spriteCommon_ = spriteCommon;

	textureSrvHandleCPU = spriteCommon_->GetDirectXCommon()->GetSRVCPUDescriptorHandle(1);
	textureSrvHandleGPU = spriteCommon_->GetDirectXCommon()->GetSRVGPUDescriptorHandle(1);



	//Sprite
	vertexResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(VertexData) * 4);

	//リソースの先頭アドレス
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	//頂点サイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);


	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { 0.0f,360.0f,0.0f,1.0f };//0
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[1].position = { 0.0f,0.0f,0.0f,1.0f };//1,3
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[2].position = { 640.0f,360.0f,0.0f,1.0f };//2,5
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[3].position = { 640.0f,0.0f,0.0f,1.0f };//4
	vertexData[3].texcoord = { 1.0f,0.0f };

	
	
	//Index
	indexResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(uint32_t) * 6);

	//リソースの先頭アドレス
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	//頂点サイズ
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 1;
	indexData[4] = 3;
	indexData[5] = 2;








	//spriteのリソース
	materialResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(Material));

	//書き込むためのアドレス
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//色の設定
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = false;
	materialData->uvTransform = MakeIdentity4x4();




	//座標変換行列	
	
	transformationMatrixResource = spriteCommon_->GetDirectXCommon()->CreateBufferResource(sizeof(TransformationMatrix));
	
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();

}


void Sprite::Update() {

	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	//Matrix4x4 cameraMatrixSprite = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	//Matrix4x4 projectionMatrixSprite = MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 100.0f);
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, (float)WinApp::kClientWidth, (float)WinApp::kClientHeight, 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));

	transformationMatrixData->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixData->World = worldMatrixSprite;

	//Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	//uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
	//uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	//materialData->uvTransform = uvTransformMatrix;

}

void Sprite::Draw() {
	if (IsSprite) {
		spriteCommon_->GetDirectXCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
		spriteCommon_->GetDirectXCommon()->GetCommandList()->IASetIndexBuffer(&indexBufferView);

		spriteCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress()); //rootParameterの配列の0番目 [0]

		spriteCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

		spriteCommon_->GetDirectXCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU);

		spriteCommon_->GetDirectXCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
}


void Sprite::Imgui() {
	//if (ImGui::TreeNode("Sprite")) {
	//	ImGui::Checkbox("IsSprite", &IsSprite);

	//	if (IsSprite) {
	//		ImGui::InputFloat("SpriteX", &transformSprite.translate.x);
	//		ImGui::SliderFloat("SliderSpriteX", &transformSprite.translate.x, 0.0f, 1000.0f);

	//		ImGui::InputFloat("SpriteY", &transformSprite.translate.y);
	//		ImGui::SliderFloat("SliderSpriteY", &transformSprite.translate.y, 0.0f, 600.0f);

	//		ImGui::InputFloat("SpriteZ", &transformSprite.translate.z);
	//		ImGui::SliderFloat("SliderSpriteZ", &transformSprite.translate.z, 0.0f, 0.0f);


	//		ImGui::DragFloat2("UVTranlate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	//		ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	//		ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
	//	}
	//	ImGui::TreePop();
	//}
}