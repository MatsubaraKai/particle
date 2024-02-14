#include "GameScene.h"
#define _USE_MATH_DEFINES
#include<math.h>

void GameScene::Init()
{
	input = Input::GetInstance();
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
		sceneTime = 0;
	}
}
void GameScene::Draw()
{
}
void GameScene::Release() {
}
// ゲームを終了
int GameScene::GameClose()
{
	return false;
}
