#include "TitleScene.h"
#include "ImGuiCommon.h"


void TitleScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	input = Input::GetInstance();

	
	sprite = new Sprite();
	sprite->Initialize({1.0f,1.0f,1.0f,1.0f});
	textureHandle = TextureManager::StoreTexture("Resources/Title.png");
}

void TitleScene::Update()
{
	
	sceneTime++;
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = CLEAR;
		sceneTime = 0;
	}
	////カメラの更新
	camera->Update();
	sprite->SetPosition({0.0f,0.0f});
	sprite->SetSize({ 1280.0f,720.0f });
	sprite->Update();
}
void TitleScene::Draw()
{
	sprite->Draw(textureHandle, {1.0f,1.0f,1.0f,1.0f});
}

void TitleScene::Release() {
	delete car_;
}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}