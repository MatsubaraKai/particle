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

class TitleScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
	void StartFadeIn();
	void UpdateFadeIn();

	WorldTransform worldTransform;
	WorldTransform worldTransform2;
private:
	
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* fadeSprite = nullptr;
	//変数
	Material material;
	uint32_t fadeTex;
	uint32_t textureHandle;
	uint32_t textureHandle2;
	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Emitter demoEmitter_;
	RandRangePro demoRandPro;
	PostProcess* postProcess_ = nullptr;
	float alpha = 0;
	bool isFadingIn = false;
};


