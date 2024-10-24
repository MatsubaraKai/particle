#pragma once
#include "Vector4.h"
#include "Vector3.h"

/**
* @file DirectionalLight.h
* @brief 平行光源を表す構造体
*/
struct DirectionalLight
{
    /// <summary>
    /// 光の色
    /// </summary>
    Vector4 color;

    /// <summary>
    /// 光の方向
    /// </summary>
    Vector3 direction;

    /// <summary>
    /// 光の強度
    /// </summary>
    float intensity;
};
