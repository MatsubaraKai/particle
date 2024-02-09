#pragma once
#include "IScene.h"
#include "Input.h"
#include "Camera.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "Model.h"
#include "TextureManager.h"
#include "../Car/Car.h"
#include "../Floor/Floor.h"
#define STAGE_MAX 10

class SlectScene:public IScene
{
public:

	void Init() override;
	void Update()override;
	void Draw()override;
	void Release()override;
	int GameClose()override;
private:

	//メンバ
	Input* input_ = nullptr;
	int sceneTime = 0;
	Camera* camera_ = nullptr;
	Car* car_ = nullptr;
	Floor* floor_ = nullptr;
	Sprite* PushSprite_ = nullptr;
	Sprite* SlectSprite_ = nullptr;
	Model* model_ = {};
	Vector4 color = { 1,1,1,1 };
	uint32_t texture_ = 1;
	uint32_t PushTexture_ = 1;

	WorldTransform worldTransform_[STAGE_MAX] = {};
	WorldTransform PushTransform_ = {};

	int SelectNumber = 0;
	bool SelectLock = false;

};

