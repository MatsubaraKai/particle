#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Particle.h"
#include "TitleCar.h"
#include "Sprite.h"
#include "TextureManager.h"
class TitleScene : public IScene
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
	TitleCar* car_ = nullptr;
	Sprite* sprite = nullptr;
	uint32_t textureHandle;
};

