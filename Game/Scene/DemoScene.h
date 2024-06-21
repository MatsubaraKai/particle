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

class DemoScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void PostDraw()override;
	void Release()override;
	int GameClose()override;
	void StartFadeOut();
	void UpdateFadeOut();
	void StartFadeIn();
	void UpdateFadeIn();

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
	Material material2;


private:
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* demoSprite = nullptr;
	Sprite* fadeSprite = nullptr;

	//変数
	uint32_t textureHandle;
	uint32_t textureHandle2;
	uint32_t fadeTex;
	Particle* particle = nullptr;
	Particle* particle2 = nullptr;
	Emitter demoEmitter_;
	RandRangePro demoRandPro;

	PostProcess* postProcess_ = nullptr;

	float alpha = 0;
	bool isFadeOut = false;
	bool isFadingIn = false;

	float angle_ = 0.0f;
	float PlayerSpeed = 0.05f;
	float PlayerJumpSpeed = 0.08f;
	float rotateSize_ = 0.05f;
	bool flag = false;
	float flagcount = 0;
};

