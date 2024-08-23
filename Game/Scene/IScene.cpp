#include "IScene.h"

// タイトルシーンで初期化
int IScene::sceneNo = TITLE;
bool IScene::TitleRoop = false;
bool IScene::DemoRoop = false;
bool IScene::GameRoop = false;
bool IScene::Game2Roop = false;
bool IScene::Game3Roop = false;

// 仮想デストラクタの定義
// 純粋仮想関数化していないので、ここで定義できる
IScene::~IScene() {}

// シーン番号のゲッター
int IScene::GetSceneNo() { return sceneNo; }
void IScene::SetSceneNo(int sceneno) {
	sceneNo = sceneno;
}