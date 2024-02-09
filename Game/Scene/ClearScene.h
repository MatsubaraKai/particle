#pragma once
#include "IScene.h"
#include "Input.h"
#include "Camera.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "Model.h"
#include "TextureManager.h"
#include "../Car/Car.h"
#include "Particle.h"
#define STAGE_MAX 10
class ClearScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Release()override;
	int GameClose()override;
private:
	int sceneTime = 0;
	Input* input = nullptr;

	//メンバ
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	Car* car_ = nullptr;
	Sprite* sprite_ = nullptr;
	Model* model2_ = {};
	Model* model_ = nullptr;
	Vector4 color = { 1,1,1,1 };

	uint32_t texture_ = 1;
	uint32_t texture2_ = 1;

	WorldTransform modelWorldTransform_ = {};
	WorldTransform modelWorldTransform2_ = {};
	WorldTransform worldTransform_ = {};

	Particle* particle = nullptr;

	Emitter lightEmitter_;

	RandRangePro rear;
	int SelectNumber = 0;
	bool SelectLock = false;

	Vector3 cameraPos;
};
;

