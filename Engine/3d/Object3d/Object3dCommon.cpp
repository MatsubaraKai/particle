#include "Object3dCommon.h"

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
