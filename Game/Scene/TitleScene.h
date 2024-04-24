#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Particle.h"
#include "TitleCar.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Audio.h"
class TitleScene : public IScene
{
public:
	void Init() override;
	void Update()override;
	void Draw()override;
	void Release()override;
	int GameClose()override;
	uint32_t GetAudio() { return Audiohandle_; };
private:
	uint32_t Audiohandle_ = 0;
	int sceneTime = 0;
	Camera* camera = nullptr;
	Input* input = nullptr;
	Sprite* sprite = nullptr;
	uint32_t textureHandle;
};

