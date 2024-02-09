#pragma once
#include <memory>
#include "IScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "Input.h"
#include "Slect.h"
#include"Stage1.h"
#include"Stage2.h"
//#include "Input.h"
class GameManager
{
private:
	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> sceneArr_[7];

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_ = 0;
	int prevSceneNo_ = 0;
public:
	GameManager();
	~GameManager();

	int Run();

};