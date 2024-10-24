#pragma once
#include "ModelData.h"
#include "mathFunction.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/**
* @file Animation.h
* @brief 3Dモデルのアニメーションデータを管理・処理するクラス
*/

struct AnimationData {
	/// <summary>
	/// アニメーション全体の長さ（秒単位）
	/// </summary>
	float duration = 0;
	/// <summary>
	/// ノードアニメーションのマップ。ノード名でアクセス可能
	/// </summary>
	std::unordered_map<std::string, NodeAnimation> nodeAnimations;
};

class Animation
{
public:
	/// <summary>
	/// アニメーションファイルを読み込む関数
	/// </summary>
	/// <param name="directoryPath">アニメーションファイルのディレクトリパス</param>
	/// <param name="filePath">アニメーションファイルのファイルパス</param>
	/// <returns>読み込んだアニメーションデータ</returns>
	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// アニメーションの初期化処理
	/// </summary>
	/// <param name="directoryPath">アニメーションファイルのディレクトリパス</param>
	/// <param name="filePath">アニメーションファイルのファイルパス</param>
	void Init(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// アニメーションの更新処理
	/// </summary>
	void Updata();

	/// <summary>
	/// アニメーションの現在の行列を取得する
	/// </summary>
	/// <returns>アニメーション行列</returns>
	Matrix4x4 GetAnimationMatrix() { return matrix_; }

private:
	/// <summary>
	/// 読み込んだアニメーションデータ
	/// </summary>
	AnimationData animation_;

	/// <summary>
	/// アニメーションの行列
	/// </summary>
	Matrix4x4 matrix_;

	/// <summary>
	/// アニメーションの現在の再生時間
	/// </summary>
	float animationTime = 0.0f;
};