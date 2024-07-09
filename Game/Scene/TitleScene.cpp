#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include <iostream>
#include <cmath>
#define M_PI 3.141592
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

	worldTransform.Initialize();
	worldTransform.translation_.x = 0;
	worldTransform2.Initialize();
	worldTransform2.translation_.x = 5;
	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();

	particle = new Particle();
	particle2 = new Particle();

	RandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{0.0f,2.0f}
	};

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	Emitter_.count = 6;
	Emitter_.frequency = 0.02f;
	Emitter_.frequencyTime = 0.0f;
	Emitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(Emitter_);
	particle2->Initialize(Emitter_);
}

void TitleScene::Update()
{
	sceneTime++;
	////カメラの更新
	camera->Update();
	camera->CameraDebug();
	fadeSprite->Update();
	fadeSprite->SpriteDebug("1");
	ImGui::Begin("color");
	float color[4] = { material.color.x,material.color.y,material.color.z,material.color.w };
	ImGui::DragFloat4("color", color, 0.01f);
	material.color = { color[0],color[1],color[2],color[3] };
	ImGui::End();
	ImGui::Begin("Space:FadeIn");
	ImGui::Checkbox("FadeIn", &isFadingIn);
	ImGui::End();
	if (input->TriggerKey(DIK_SPACE)) {
		StartFadeIn();
	}
	if (isFadingIn)
	{
		UpdateFadeIn();
	}

	setRainbowColor(time, material.color.x, material.color.y, material.color.z);
	time += deltaTime; // 時間を進める
}
void TitleScene::Draw()
{
	particle->Draw(Emitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, RandPro, false);
	particle2->Draw(Emitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, RandPro, false);
	fadeSprite->Draw(textureHandle2, material.color);
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
void TitleScene::setRainbowColor(float time, float& red, float& green, float& blue) {
	float frequency = 0.5f; // 色の変化の速度を調整するための周波数
	float amplitude = 0.5f; // 色の振幅を調整する係数

	red = amplitude * std::sin(frequency * time + 0) + 0.5f;
	green = amplitude * std::sin(frequency * time + 2 * static_cast<float>(M_PI) / 3) + 0.5f;
	blue = amplitude * std::sin(frequency * time + 4 * static_cast<float>(M_PI) / 3) + 0.5f;
}