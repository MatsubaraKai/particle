#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "IScene.h"
#include "TextureManager.h"
#include "Audio.h"
#include "Particle.h"
#include "Sprite.h"
class Floor
{
public:
	Floor();
	~Floor();
	void Init();
	void Update();
	void Draw(Camera* camera);
	void Release();
private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t texture_ = 1;
	Input* input = nullptr;
	Vector4 color = { 0.3f,0.3f,0.3f,1.0f };
};

