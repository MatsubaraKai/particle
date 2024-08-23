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

class STAGE1 :public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;

	WorldTransform worldTransformPa;
	WorldTransform worldTransformPa2;
	WorldTransform TenQTransform;
	Object3d* TenQOBJ = nullptr;
	Object3d* TextOBJ = nullptr;
private:
	int sceneTime = 0;
	int sceneTime1 = 0;
	int selectedIndex1 = 0;
	int selectedIndex2 = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Fade* fade = nullptr;

	std::vector<Object3d*> ConeObject_;
	std::vector<Object3d*> StarObject_;
	Object3d* Number = nullptr;
	Particle* particle = nullptr;
	Particle* particle2 = nullptr;

	PostProcess* postProcess_ = nullptr;
	//変数
	uint32_t FADEtextureHandle;
	uint32_t WHITEtextureHandle;
	uint32_t BLUEtextureHandle;
	uint32_t CONEtextureHandle;
	uint32_t TENQtextureHandle;
	uint32_t GRIDtextureHandle;
	uint32_t STARtextureHandle;

	Emitter ParticleEmitter_;
	RandRangePro demoRandPro;
	float rotateSize_ = 1.057f;
	bool effectFlag = true;
	bool effect = false;
	bool effect2 = false;
	bool isOnFloor = false;
	bool isGetStar = false;
	bool isClear = false;
	bool isTitle = false;
	bool isFadeInStarted = false;
	int starCount = 5;
};

