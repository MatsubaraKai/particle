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
private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* demoSprite = nullptr;
	Sprite* fadeSprite = nullptr;

	std::vector<Object3d*> object3d_;
	Object3d* object3d = nullptr;
	Object3d* object3d2 = nullptr;

	Particle* particle = nullptr;
	Particle* particle2 = nullptr;

	PostProcess* postProcess_ = nullptr;
	//変数
	uint32_t fadeTex;
	uint32_t textureHandle;
	uint32_t textureHandle2;
	
	Material material;
	Material material2;

	WorldTransform worldTransform;
	WorldTransform worldTransform2;

	
	Emitter demoEmitter_;
	RandRangePro demoRandPro;

	float alpha = 0;
	bool isFadeOut = false;
	bool isFadingIn = false;
	int id = 0;
	float rotateSize_ = 1.057f;
};



