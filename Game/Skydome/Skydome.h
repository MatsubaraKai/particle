#pragma once
#include "WorldTransform.h"
#include "Camera.h"
#include "Input.h"
#include "IScene.h"
#include "TextureManager.h"
#include "Audio.h"
#include "Particle.h"
#include "Sprite.h"
#include "Model.h"
#include "Sphere.h"
#include "Particle.h"
class Skydome
{
public:

	Skydome();
	~Skydome();
	void Init();
	void Update();
	void Draw(Camera* camera);
	void Release();

	Vector3 GetWorldTransform() {
		return worldTransform_.translation_;
	}

private:
	uint32_t texture_ = 1;
	Input* input = nullptr;
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	Model* model_ = nullptr;
	Camera* camera = nullptr;

public:
	WorldTransform worldTransform_;

};
