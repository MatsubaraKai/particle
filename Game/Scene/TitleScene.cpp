#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include <iostream>
#include <cmath>
#include "mathFunction.h"

void TitleScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.x = 3.5f;
	cameraPos.y = 3.0f;
	cameraPos.z = -15.0f;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();
	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/white.png");
	fadeTex = TextureManager::StoreTexture("Resources/black.png");

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

	fade = new Fade;
	fade->Init(fadeTex);
	fade->StartFadeOut();
}

void TitleScene::Update()
{
	fade->UpdateFade();
	if (input->TriggerKey(DIK_SPACE)) {
		fade->StartFadeIn();
	}
	if (fade->IsFadeOutComplete()) {
		sceneNo = 3;
	}
	sceneTime++;
	////カメラの更新
	camera->Update();
	camera->CameraDebug();
	camera->Move();

	particle->Particledebug("uv",worldTransform);
	particle2->Particledebug("white",worldTransform2);

	
	
	ImGui::Begin("color");
	float color[4] = { fade->material.color.x,fade->material.color.y,fade->material.color.z,fade->material.color.w };
	ImGui::DragFloat4("color", color, 0.01f);
	fade->material.color = { color[0],color[1],color[2],color[3] };
	ImGui::End();
	ImGui::Begin("Space:FadeIn");
	if (ImGui::Button("FadeIn ")) {
		fade->StartFadeIn();
	}
	ImGui::Text("Now Scene : %d", sceneNo);
	ImGui::End();
}
void TitleScene::Draw()
{
	particle->Draw(Emitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, RandPro, false);
	particle2->Draw(Emitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, RandPro, false);
	
	fade->Draw();
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
