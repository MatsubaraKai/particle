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
#include "DirectXMath.h"


class TitleScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;

	WorldTransform worldTransformPa;
	WorldTransform worldTransformPa1;
	WorldTransform worldTransformPa2;
	WorldTransform TenQTransform;
private:
	int sceneTime = 0;
	int sceneTime1 = 0;
	int selectedIndex1 = 0;
	int selectedIndex2 = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Fade* fade = nullptr;

	std::vector<Object3d*> ConeObject_;

	Object3d* TenQOBJ = nullptr;
	Object3d* TitleOBJ = nullptr;
	Object3d* TitleOBJ2 = nullptr;
	Object3d* TextOBJ3 = nullptr;
	Object3d* TextOBJ4 = nullptr;
	Object3d* TextOBJ5 = nullptr;
	Object3d* TextOBJ6 = nullptr;

	Particle* particle = nullptr;
	Particle* particle1 = nullptr;
	Particle* particle2 = nullptr;
	Particle* particle3 = nullptr;

	PostProcess* postProcess_ = nullptr;
	//変数
	uint32_t FADEtextureHandle;
	uint32_t WHITEtextureHandle;
	uint32_t BLUEtextureHandle;
	uint32_t CONEtextureHandle;
	uint32_t GRIDtextureHandle;
	uint32_t TENQtextureHandle;

	Emitter ParticleEmitter_;
	RandRangePro demoRandPro;
	float rotateSize_ = 1.057f;
	bool effectFlag = true;
	bool effect = false;
	bool effect2 = false;
	bool isOnFloor = false;
	bool isDemo = false;//3
	bool isGame = false;//1
	bool isGame2 = false;//2
	bool isClear = false;
	bool isFadeInStarted = false;

};


