#pragma once
#include <vector>
#include "VertexData.h"
#include "MaterialData.h"
#include "Quaternion.h"
#include <unordered_map>
#include <Matrix4x4.h>
#include "Skeleton.h"

/**
* @file ModelData.h
* @brief 3Dモデルデータおよびアニメーションデータを扱う構造体群
*/

// キーフレームを表すテンプレート構造体
template <typename tValue>
struct Keyframe {
	float time;  // キーフレームの時間
	tValue value;  // キーフレームの値 (例: ベクトルやクォータニオン)
};

// Vector3 型のキーフレーム
using KeyFrameVector3 = Keyframe<Vector3>;

// クォータニオン型のキーフレーム
using KeyFrameQuaternion = Keyframe<Quaternion>;

// アニメーションカーブを表すテンプレート構造体
template <typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;  // キーフレームのリスト
};

// ノードアニメーションデータ
struct NodeAnimation {
	AnimationCurve<Vector3> translate;  // 平行移動アニメーション
	AnimationCurve<Quaternion> rotate;  // 回転アニメーション
	AnimationCurve<Vector3> scale;  // スケールアニメーション
};
