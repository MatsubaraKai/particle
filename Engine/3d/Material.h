#pragma once
#include "Vector4.h"
#include "Matrix4x4.h"
#include <cstdint>

/**
* @file Material.h
* @brief マテリアルを表す構造体
*/
struct Material {
    /// <summary>
    /// マテリアルの色
    /// </summary>
    Vector4 color;

    /// <summary>
    /// ライティングを有効にするかどうかのフラグ
    /// </summary>
    int32_t enableLighting;

    /// <summary>
    /// パディング（メモリ整列のための余白）
    /// </summary>
    float padding[3];

    /// <summary>
    /// UV変換行列
    /// </summary>
    Matrix4x4 uvTransform;

    /// <summary>
    /// 光沢度（スペキュラーの強さ）
    /// </summary>
    float shininess;
};
