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
class DemoScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
	void StartFadeIn();
	void StartFadeOut();
	void UpdateFadeIn();
	void UpdateFadeOut();

	WorldTransform worldTransform;
	WorldTransform worldTransform2;
	Material material2;
	WorldTransform TenQTransform;
	Object3d* TenQOBJ = nullptr;
private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* demoSprite = nullptr;
	Sprite* fadeSprite = nullptr;

	std::vector<Object3d*> object3d_;
	Object3d* GridOBJ = nullptr;
	Object3d* object3d2 = nullptr;
	Object3d* ConeOBJ = nullptr;
	Object3d* WallOBJ = nullptr;

	Particle* particle = nullptr;
	Particle* particle2 = nullptr;

	PostProcess* postProcess_ = nullptr;
	//変数
	uint32_t FADEtextureHandle;
	uint32_t UVtextureHandle;
	uint32_t WHITEtextureHandle;
	uint32_t CONEtextureHandle;
	uint32_t TENQtextureHandle;
	uint32_t GRIDtextureHandle;

	Material material;
	
	WorldTransform GridTransform;
	
	Emitter demoEmitter_;
	RandRangePro demoRandPro;

	float alpha = 0;
	bool isFadeOut = false;
	bool isFadingIn = false;
	float rotateSize_ = 1.057f;
};



