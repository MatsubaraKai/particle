#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>

struct Vector3 final {
    float x, y, z;

    // - 演算子のオーバーロード
    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }
};