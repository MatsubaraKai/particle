#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "IScene.h"
#include "TextureManager.h"
#include "Audio.h"
#include "Particle.h"
#include "Sprite.h"
#include "Particle.h"

enum DriveMode {
	NormalMode,
	DriftMode
};



//enum SpeedMode {
//	Normal,
//	Accel
//};

class Car
{
public:

	Car();
	~Car();
	void Init();
	void Update();
	void Draw(Camera* camera);
	void Release();

	// 車の発車
	void Depart();
	void Move();
	void Drift();
	void Accel();
	void SetDriveMode(int driveMode);
	Vector3 GetWorldTransform() {
		return worldTransform_.translation_;
	}
	int GetDriveMode() {
		return driveMode_;
	}
private:
	Model* model_ = nullptr;
	uint32_t texture_ = 1;
	uint32_t texture2_ = 2;
	uint32_t texture3_ = 3;
	Input* input = nullptr;
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Sprite* sprite_ = nullptr;
	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Particle* particle3 = nullptr;
	Particle* particle4 = nullptr;
	Camera* camera = nullptr;

	int driveMode_ = NormalMode;
	Emitter smokeEmitter_;
	Emitter lightEmitter_;

public:
	WorldTransform worldTransform_;
	float rotate_;
	float Speed = 0.0f;
	float NormalSpeed = 1.5f;
	float ShiftSpeed = 2.0f;
	float DriftSpeed = 1.2f;
	bool moveFlag_ = false;
	Vector2 move;
	RandRangePro rearLeft;
	RandRangePro rearRight;


	//Particle* particle = nullptr;
	//std::list<Particle::ParticlePro> particles_;

};

//// 車の発車
//void Car::Depart()
//{
//	XINPUT_STATE Gamepad;
//	if (!input->GetJoystickState(0, Gamepad)) {
//		return;
//	}
//	if (Gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
//		moveFlag_ = true;
//	}
//
//}
