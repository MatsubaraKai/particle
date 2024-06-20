﻿#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Xinput.h"
#include "Particle.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "PostProcess.h"

class DemoScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;

	void Move();
	void Jump();
	float Lerp(const float& a, const float& b, float t);
	float LerpShortAngle(float a, float b, float t);
	float LerpShortTranslate(float a, float b, float t);
	float Length(const Vector3& v);
	WorldTransform worldTransform;
	WorldTransform worldTransform2;
	Object3d* object3d = nullptr;
	Object3d* object3d2 = nullptr;
	Object3d* object3d3 = nullptr;
	Material material;

private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* demoSprite = nullptr;

	//変数
	uint32_t textureHandle;
	uint32_t textureHandle2;


	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Emitter demoEmitter_;
	RandRangePro demoRandPro;

	PostProcess* postProcess_ = nullptr;

	float angle_ = 0.0f;
	float PlayerSpeed = 0.05f;
	float PlayerJumpSpeed = 0.08f;
	float rotateSize_ = 0.05f;
	bool flag = false;
	float flagcount = 0;
};

