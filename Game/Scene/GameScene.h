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

class GameScene :public IScene
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
	Material material;
private:
	int sceneTime = 0;
	Camera *camera = nullptr;
	Input *input = nullptr;

	PostProcess *postProcess_ = nullptr;
	Sprite *fadeSprite = nullptr;

	uint32_t fadeTex;

	float alpha = 0;
	bool isFadeOut = false;
	bool isFadingIn = false;
};

