#pragma once

#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Input.h"
#include "Particle.h"
class Stage2 : public IScene
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
	Player* player_ = nullptr;
	Input* input = nullptr;

};

