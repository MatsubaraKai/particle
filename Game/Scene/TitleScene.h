#pragma once
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
#include "Fade.h"

class TitleScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
	
	WorldTransform worldTransform;
	WorldTransform worldTransform2;
private:
	
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Fade *fade = nullptr;
	//変数
	uint32_t fadeTex;
	uint32_t textureHandle;
	uint32_t textureHandle2;
	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Emitter Emitter_;
	RandRangePro RandPro;
	PostProcess* postProcess_ = nullptr;

};


