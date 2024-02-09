#include "Rear_right_tire.h"
#include "ImGuiCommon.h"

#define _USE_MATH_DEFINES
#include<math.h>
Rear_right_tire::Rear_right_tire() {

}

Rear_right_tire::~Rear_right_tire() {

}

void Rear_right_tire::Init() {
	input = Input::GetInstance();
	worldTransform_.Initialize();
	worldTransform_.translation_.y += 0.05f;
	worldTransform_.translation_.z += 10.0f;
	texture_ = TextureManager::StoreTexture("Resources/uvChecker.png");
	model_ = new Model();
	model_->Initialize("Resources/demo_car", "rear_right_tire.obj", color);

	Speed = 10;
}

void Rear_right_tire::Update() {

	float theta = (rotate_ / 2.0f) * (float)M_PI;
	move = { cosf(theta),sinf(theta) };
	if (!input->PushKey(DIK_S)) {
		float theta = (rotate_ / 2.0f) * (float)M_PI;
		move = { cosf(theta),sinf(theta) };
		worldTransform_.rotation_.y = theta;
	}
	else if (input->PushKey(DIK_S)) {
		if (rotate_ > 0) {
			float theta = (rotate_ + 0.1f / 2.0f) * (float)M_PI;
			move = { cosf(theta),sinf(theta) };
			worldTransform_.rotation_.y = theta;
		}
		else if (rotate_ < 0) {
			float theta = (rotate_ - 0.1f / 2.0f) * (float)M_PI;
			move = { cosf(theta),sinf(theta) };
			worldTransform_.rotation_.y = theta;
		}
	}
	//worldTransform_.translation_.z += Speed;
	if (input->PushKey(DIK_LSHIFT)) {
		Speed = ShiftSpeed;
	}
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
	if (input->PushKey(DIK_A) && worldTransform_.rotation_.y >= -1.5f) {
		rotate_ -= 0.04f;
	}
	else  if (input->PushKey(DIK_D) && worldTransform_.rotation_.y <= 1.5f) {
		rotate_ += 0.04f;
	}
	if (worldTransform_.rotation_.y >= 1.5f) {
		worldTransform_.rotation_.y = 1.5f;
	}
	if (worldTransform_.rotation_.y <= -1.5f) {
		worldTransform_.rotation_.y = -1.5f;
	}
	Depart();
	if (input->TriggerKey(DIK_W)) {
		moveFlag_ = true;
	}

	if (moveFlag_ && input->PushKey(DIK_S)) {
		worldTransform_.translation_.x += DriftSpeed * move.y;
		worldTransform_.translation_.z += DriftSpeed * move.x;
	}
	else if (moveFlag_) {
		worldTransform_.translation_.x += Speed * move.y;
		worldTransform_.translation_.z += Speed * move.x;

	}
	worldTransform_.UpdateMatrix();
}

void Rear_right_tire::Draw(Camera* camera) {
	model_->Draw(worldTransform_, texture_, camera, color);
}

void Rear_right_tire::Release()
{
}

void Rear_right_tire::Move()
{
	XINPUT_STATE joyState;
	/*switch (driveMode_) {
	}*/
	if (Input::GetInstance()->GetJoystickState(joyState)) {


		if (worldTransform_.rotation_.y >= -1.5f || worldTransform_.rotation_.y <= 1.5f) {
			rotate_ += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * 0.04f;
			if (worldTransform_.rotation_.y >= 1.5f) {
				worldTransform_.rotation_.y = 1.5f;
			}
			if (worldTransform_.rotation_.y <= -1.5f) {
				worldTransform_.rotation_.y = -1.5f;
			}
		}

		//rotate_ += 0.04f;

	}
}

void Rear_right_tire::Accel()
{
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		Speed = ShiftSpeed;
	}
	else
	{
		Speed = NormalSpeed;
	}
}

void Rear_right_tire::Depart()
{
	XINPUT_STATE Gamepad{};
	if (!Input::GetInstance()->GetJoystickState(Gamepad)) {
		return;
	}
	if (Gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

		moveFlag_ = true;

	}

	else {

	}

}
