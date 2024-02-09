#pragma once
#include "IScene.h"
#include "Input.h"
#include "../Car/Car.h"
#include "ImGuiCommon.h"
#include "Transform.h"
class GameScene :public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Release()override;
	int GameClose()override;

	void Depart();
	void Accel();
private:
	Input* input = nullptr;
	int sceneTime = 0;
	// Carが動く
	bool moveFlag = false;

	bool DriftFlag = false;
	bool AccelFlag = false;

	Camera* camera = nullptr;
	Car* car_ = nullptr;
	Transform DriftCamera;
	Transform AccelCamera;
	Transform NormalCamera;
	Transform AccelDriftCamera;
};

