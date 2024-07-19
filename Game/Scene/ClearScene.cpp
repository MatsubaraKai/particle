#include "ClearScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

void ClearScene::Init()
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
	fadeSprite = new Sprite();
	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });
	material.color = { 1.0f,1.0f,1.0f,0.0f };
	material.enableLighting = true;

	StartFadeOut();
}
void ClearScene::Update()
{
	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}
	if (input->TriggerKey(DIK_SPACE)) {
		StartFadeIn();
	}
	if (isFadingIn == true)
	{
		UpdateFadeIn();
	}

	camera->Update();
	camera->Move();
	fadeSprite->Update();

	sceneTime++;
	///////////////Debug///////////////
	camera->CameraDebug();

	fadeSprite->SpriteDebug("fadesprite");
	ImGui::Begin("color");
	float color[4] = { material.color.x,material.color.y,material.color.z,material.color.w };
	ImGui::DragFloat4("color", color, 0.01f);
	material.color = { color[0],color[1],color[2],color[3] };
	ImGui::End();
	ImGui::Begin("Space:FadeIn");
	ImGui::Checkbox("FadeIn", &isFadingIn);
	ImGui::End();

}
void ClearScene::Draw()
{
	fadeSprite->Draw(fadeTex, material.color);
}

void ClearScene::PostDraw()
{
	postProcess_->Draw();
}

void ClearScene::Release() {

}


// ゲームを終了
int ClearScene::GameClose()
{
	return false;
}

void ClearScene::StartFadeOut()
{
	isFadeOut = true;
	alpha = 1.0f;
	material.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void ClearScene::UpdateFadeOut()
{
	alpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha <= 0.0f)
	{
		// フェードイン完了時の処理
		isFadeOut = false;
	}
}


void ClearScene::StartFadeIn()
{
	isFadingIn = true;
	alpha = 0.0f;
	material.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void ClearScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		alpha = 0.0f;
		sceneNo = 0;
	}
}
