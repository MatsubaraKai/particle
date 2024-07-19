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
	fadeSprite = new Sprite();
	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });
	material.color = { 1.0f,1.0f,1.0f,0.0f };
	material.enableLighting = true;

	StartFadeOut();
}

void GameScene::Update()
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
void GameScene::Draw()
{
	fadeSprite->Draw(fadeTex, material.color);
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

void GameScene::StartFadeOut()
{
	isFadeOut = true;
	alpha = 1.0f;
	material.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void GameScene::UpdateFadeOut()
{
	alpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha <= 0.0f)
	{
		// フェードイン完了時の処理
		isFadeOut = false;
	}
}


void GameScene::StartFadeIn()
{
	isFadingIn = true;
	alpha = 0.0f;
	material.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void GameScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		alpha = 0.0f;
		sceneNo = 2;
	}
}
