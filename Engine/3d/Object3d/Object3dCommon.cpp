#include "Object3dCommon.h"
/**
* @file Object3dCommon.cpp
* @brief 3Dオブジェクトに共通する処理を管理するクラス
*/
void Object3dCommon::Init()
{
}

void Object3dCommon::Update()
{
}

void Object3dCommon::Draw()
{
}

Object3dCommon* Object3dCommon::GetInstance()
{
    static Object3dCommon instance;
    return &instance;

}
