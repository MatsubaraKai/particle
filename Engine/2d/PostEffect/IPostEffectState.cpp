#include "IPostEffectState.h"
/**
* @file IPostEffectState.cpp
* @brief IPostEffectState
*/
// タイトルシーンで初期化
int IPostEffectState::effectNo_ = kOutlineBlue;

// 仮想デストラクタの定義
// 純粋仮想関数化していないので、ここで定義できる
IPostEffectState::~IPostEffectState() {}

// エフェクト番号のゲッター
int IPostEffectState::GetEffectNo() { return effectNo_; }

// エフェクト番号のセッター
void IPostEffectState::SetEffectNo(int effectNo) { effectNo_ = effectNo; }
