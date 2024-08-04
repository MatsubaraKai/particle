#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Particle.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
#include "PostProcess.h"
#include "Transform.h"
#include "Fade.h"
#include "Skybox.h"
class DemoScene : public IScene
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
	WorldTransform worldTransformSKY;
	WorldTransform TenQTransform;
	Object3d* TenQOBJ = nullptr;
private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Fade *fade = nullptr;

	std::vector<Object3d*> object3d_;
	Object3d* GridOBJ = nullptr;
	Object3d* SkyBoxOBJ = nullptr;
	Object3d* PlayerOBJ = nullptr;
	Object3d* ConeOBJ = nullptr;
	Object3d* WallOBJ = nullptr;
	Skybox* skybox_ = nullptr;
	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Material material;

	PostProcess* postProcess_ = nullptr;
	//変数
	uint32_t FADEtextureHandle;
	uint32_t UVtextureHandle;
	uint32_t WHITEtextureHandle;
	uint32_t CONEtextureHandle;
	uint32_t TENQtextureHandle;
	uint32_t GRIDtextureHandle;
	uint32_t SKYtextureHandle;
	
	WorldTransform GridTransform;
	
	Emitter ParticleEmitter_;
	RandRangePro demoRandPro;
	float rotateSize_ = 1.057f;
	bool effectFlag = true;
	bool effect = false;
	bool effect2 = false;
};



