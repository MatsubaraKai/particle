#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Particle.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Model.h"
class DemoScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Release()override;
	int GameClose()override;

private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* demoSprite = nullptr;

	//変数
	uint32_t textureHandle;
	uint32_t textureHandle2;
	Object3d* object3d = nullptr;
	Object3d* object3d2 = nullptr;
	Material material;

	WorldTransform worldTransform;
	WorldTransform worldTransform2;

	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Emitter demoEmitter_;
	RandRangePro demoRandPro;
};