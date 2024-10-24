#pragma once
#include <vector>
#include <map>
#include "Model.h"
#include "AnimationModel.h"

/**
* @file ModelManager.h
* @brief モデルを管理するシングルトンクラス
*/

class ModelManager
{
private:
	// コンストラクタとデストラクタをプライベートにしてシングルトンを実装
	ModelManager() = default;
	~ModelManager() = default;

	// コピー代入演算子を削除（シングルトンは複製不可）
	const ModelManager& operator=(const ModelManager&) = delete;

public:
	// シングルトンインスタンスを取得する
	static ModelManager* GetInstance();

public:
	// 初期化
	void init();
	// 更新
	void Update();
	// リリース（モデルデータの削除など）
	void Release();
	// ファイナライズ（終了処理）
	void Finalize();

private:
	DirectXCommon* dxCommon_;  // DirectXの共通処理を保持

	// モデルデータを保持するマップ
	std::map<std::string, std::unique_ptr<Model>> models_;
	// アニメーションモデルデータを保持するマップ
	std::map<std::string, std::unique_ptr<AnimationModel>> animationModels_;

public:
	/// <summary>
	/// モデルファイルを読み込む
	/// </summary>
	void LoadModel(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// アニメーションモデルファイルを読み込む
	/// </summary>
	void LoadAnimationModel(const std::string& directoryPath, const std::string& filePath);

	/// <summary>
	/// モデルを検索する
	/// </summary>
	Model* FindModel(const std::string& filePath);

	/// <summary>
	/// アニメーションモデルを検索する
	/// </summary>
	AnimationModel* FindAnimationModel(const std::string& filePath);
};
