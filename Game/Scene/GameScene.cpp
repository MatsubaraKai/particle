#include "GameScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

void GameScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	fadeTex = TextureManager::StoreTexture("Resources/black.png");
	fade = new Fade;
	fade->Init(fadeTex);
	fade->StartFadeOut();
}

void GameScene::Update()
{
	fade->UpdateFade();
	if (input->TriggerKey(DIK_SPACE)) {
		fade->StartFadeIn();
	}
	if (fade->IsFadeOutComplete()) {
		sceneNo = 2;
	}
	camera->Update();
	camera->Move(isOnFloor);

	sceneTime++;
	///////////////Debug///////////////
	camera->CameraDebug();

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
void GameScene::Draw()
{
	fade->Draw();
}

void GameScene::PostDraw()
{
	postProcess_->Draw();
}

void GameScene::Release() {

}
// ゲームを終了
int GameScene::GameClose()
{
	return false;
}