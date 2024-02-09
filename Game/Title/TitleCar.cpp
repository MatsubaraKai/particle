#include "TitleCar.h"
#include "ImGuiCommon.h"
#include "random"
#define _USE_MATH_DEFINES
#include<math.h>

TitleCar::TitleCar() {

}

TitleCar::~TitleCar() {

}

void TitleCar::Init() {
	input = Input::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_.y += 0.05f;
	worldTransform_.translation_.z += 10.0f;
	texture_ = TextureManager::StoreTexture("Resources/demo_car/CAR.png");
	texture2_ = TextureManager::StoreTexture("Resources/circle.png");
	texture3_ = TextureManager::StoreTexture("Resources/cubeRED.png");
	model_ = new Model();
	model_->Initialize("Resources/demo_car", "CAR.obj", color);

	Speed = 10;
	rearLeft = {
		{-0.9f,-0.7f},
		{0.1f,0.4f},
		{-0.5f,0.3f}
	};

	rearRight = {
		{0.7f,0.9f},
		{0.1f,0.4f},
		{-0.5f,0.3f}
	};

	smokeEmitter_.count = 6;
	smokeEmitter_.frequency = 0.02f;
	smokeEmitter_.frequencyTime = 0.0f;
	smokeEmitter_.transform.scale = { 0.2f,0.2f,0.2f };

	lightEmitter_.count = 6;
	lightEmitter_.frequency = 0.02f;
	lightEmitter_.frequencyTime = 0.0f;
	lightEmitter_.transform.scale = { 1.0f,1.0f,1.0f };

	particle = new Particle();
	particle->Initialize(smokeEmitter_);
	particle2 = new Particle();
	particle2->Initialize(smokeEmitter_);
	particle3 = new Particle();
	particle3->Initialize(smokeEmitter_);
	particle4 = new Particle();
	particle4->Initialize(smokeEmitter_);
}

void TitleCar::Update() {

	//if(Input::GetInstance()->)
	float theta = (rotate_ / 2.0f) * (float)M_PI;
	move = { cosf(theta),sinf(theta) };
	worldTransform_.rotation_.y = theta;
	Drift();
	{

	}

	//worldTransform_.translation_.z += Speed;
	/*if (input->PushKey(DIK_LSHIFT)) {
		Speed = ShiftSpeed;
	}*/
	/*else
	{
		Speed = NormalSpeed;
	}*/
	if (worldTransform_.translation_.x >= 100.0f) {
		worldTransform_.translation_.x = 100.0f;
	}
	if (worldTransform_.translation_.x <= -100.0f) {
		worldTransform_.translation_.x = -100.0f;
	}

	Move();
	Accel();
	if (input->PushKey(DIK_A) /*&& worldTransform_.rotation_.y >= -1.5f*/) {
		rotate_ -= 0.04f;
	}
	else if (input->PushKey(DIK_D) /*&& worldTransform_.rotation_.y <= 1.5f*/) {
		rotate_ += 0.04f;
	}
	/*if (worldTransform_.rotation_.y >= 1.5f) {
		worldTransform_.rotation_.y = 1.5f;
	}
	if (worldTransform_.rotation_.y <= -1.5f) {
		worldTransform_.rotation_.y = -1.5f;
	}*/
	if (input->TriggerKey(DIK_W)) {
		moveFlag_ = true;
	}
	{
		Depart();
	}

	worldTransform_.UpdateMatrix();

	ImGui::Begin("Demo_Car");
	ImGui::DragFloat3("translation_", (float*)&worldTransform_.translation_, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("rotation_", (float*)&worldTransform_.rotation_, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat3("scale_", (float*)&worldTransform_.scale_, 0.01f, -100.0f, 100.0f);
	ImGui::DragFloat4("color", &color.x, 0.01f);
	ImGui::Text("%f", theta);
	ImGui::Text("%f", rotate_);
	ImGui::End();
	//Audio::SoundLoopWave(Audio::GetIXAudio().Get(), soundData);
}

void TitleCar::Draw(Camera* camera) {

	model_->Draw(worldTransform_, texture_, camera, color);


	//particle3->Draw(lightEmitter_, { worldTransform_.translation_.x - 3 * move.y,worldTransform_.translation_.y + 0.3f,worldTransform_.translation_.z - 3 * move.x }, texture3_, camera, rearLeft, false);
	//particle4->Draw(lightEmitter_, { worldTransform_.translation_.x - 3 * move.y,worldTransform_.translation_.y + 0.3f,worldTransform_.translation_.z - 3 * move.x }, texture3_, camera, rearRight, false);
	//
	//particle->Draw(smokeEmitter_, { worldTransform_.translation_.x - 2 * move.y,worldTransform_.translation_.y - 0.2f,worldTransform_.translation_.z - 2 * move.x }, texture2_, camera, rearLeft, true);
	//particle2->Draw(smokeEmitter_, { worldTransform_.translation_.x - 2 * move.y,worldTransform_.translation_.y - 0.2f,worldTransform_.translation_.z - 2 * move.x }, texture2_, camera, rearRight, true);



	//sprite_->Draw(texture_,color);
}

void TitleCar::Release()
{

}

// 車の発車
void TitleCar::Depart()
{
	XINPUT_STATE Gamepad;
	Input::GetInstance()->GetJoystickState(Gamepad);

}

void TitleCar::Move()
{
	XINPUT_STATE joyState{};
	/*switch (driveMode_) {
	}*/
	if (Input::GetInstance()->GetJoystickState(joyState)) {
	}


	//rotate_ += 0.04f;

}
//}


void TitleCar::Drift()
{
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(joyState);

	smokeEmitter_.count = 6;


}

void TitleCar::Accel()
{
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(joyState);

	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::GetInstance()->PushKey(DIK_LSHIFT)) {
		Speed = ShiftSpeed;
		lightEmitter_.count = 10;
	}
	else
	{
		lightEmitter_.count = 0;
		Speed = NormalSpeed;
	}

}

