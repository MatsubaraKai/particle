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
	//Audio
	audio_ = Audio::GetInstance();
	soundData1_ = audio_->SoundLoad("Resources/mp3test.mp3");
	//音声再生
	audio_->SoundPlayWave(soundData1_, 0.1f, false);

}

void TitleScene::Update()
{

	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = CLEAR;
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
}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}