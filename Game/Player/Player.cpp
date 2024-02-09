#include "Player.h"
#include "ImGuiCommon.h"
#define _USE_MATH_DEFINES
#include<math.h>
Player::Player() {

}

Player::~Player() {

}

void Player::Init() {
	input = Input::GetInstance();
	worldTransform_.Initialize();
	texture_ = TextureManager::StoreTexture("Resources/circle.png");
	texture2_ = TextureManager::StoreTexture("Resources/monsterBall.png");
	soundData = Audio::SoundLoadWave("Resources/fanfare.wav");
	soundData2 = Audio::SoundLoadWave("Resources/fanfare.wav");
	model_ = new Model();
	model_->Initialize("Resources/demo_car","demo_cube.obj",color);
	//particle = new Particle();
	//particle->Initialize();
	sprite_ = new Sprite;
	sprite_->Initialize(color);
}

void Player::Update() {
	



	if (input->PushKey(DIK_W)) {
		worldTransform_.translation_.z+= 0.5f;
	}

	if (input->PushKey(DIK_S)) {
		worldTransform_.translation_.z-= 0.5f;
	}

	if (input->PushKey(DIK_J)) {
		worldTransform_.translation_.y--;
	}
	

	ImGui::Begin("Color");
	ImGui::DragFloat4("s",&worldTransform_.scale_.x,0.01f);
	ImGui::DragFloat4("r", &worldTransform_.rotation_.x,  0.01f);
	ImGui::DragFloat4("t", &worldTransform_.translation_.x,  0.01f);
	ImGui::End();
	//Audio::SoundLoopWave(Audio::GetIXAudio().Get(), soundData);
	worldTransform_.UpdateMatrix();
}

void Player::Draw(Camera *camera) {
	model_->Draw(worldTransform_, texture_, camera,color);
	//sprite_->Draw(texture_,color);
	//particle->Draw(texture_, color, camera);
}

void Player::Release()
{
	Audio::SoundUnload(soundData2);
}
