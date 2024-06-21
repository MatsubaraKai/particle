﻿#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

void TitleScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.x = 3.5f;
	cameraPos.y = 1.0f;
	cameraPos.z = -15.0f;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();
	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/white.png");
	fadeTex = TextureManager::StoreTexture("Resources/black.png");
	fadeSprite = new Sprite();
	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });
	material.color = { 1.0f,1.0f,1.0f,0.0f };
	material.enableLighting = false;
	particle = new Particle();
	particle2 = new Particle();
	worldTransform.Initialize();
	worldTransform.translation_.x = 0;
	worldTransform2.Initialize();
	worldTransform2.translation_.x = 5;
	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	demoRandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{0.0f,2.0f}
	};
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();
	demoEmitter_.count = 6;
	demoEmitter_.frequency = 0.02f;
	demoEmitter_.frequencyTime = 0.0f;
	demoEmitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(demoEmitter_);
	particle2->Initialize(demoEmitter_);
}

void TitleScene::Update()
{
	sceneTime++;
	////カメラの更新
	camera->Update();
	camera->CameraDebug();
	fadeSprite->Update();
	if (input->TriggerKey(DIK_SPACE)) {
		StartFadeIn();
	}
	if (isFadingIn)
	{
		UpdateFadeIn();
	}
}
void TitleScene::Draw()
{
	particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, demoRandPro, false);
	fadeSprite->Draw(fadeTex, material.color);
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
}

void TitleScene::Release() {

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}

void TitleScene::StartFadeIn()
{
	isFadingIn = true;
}

void TitleScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		alpha = 0.0f;
		sceneNo = 3;
	}
}