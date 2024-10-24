#include "ModelManager.h"
/**
* @file ModelManager.cpp
* @brief モデルを管理するシングルトンクラス
*/
void ModelManager::init()
{
	// 引数で受け取ってメンバ変数に記録する
	dxCommon_ = DirectXCommon::GetInstance();
}

void ModelManager::Update()
{
}

void ModelManager::Release()
{
}

void ModelManager::Finalize()
{
}

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		//  読み込み済みなら早期return
		return;
	}
	// モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model> model = std::make_unique<Model>();
	model->Initialize(directoryPath, filePath,
		{ { 1.0f,1.0f,1.0f,1.0 },true
		});
	models_.insert(std::make_pair(filePath, std::move(model)));
}

void ModelManager::LoadAnimationModel(const std::string& directoryPath, const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (animationModels_.contains(filePath)) {
		//  読み込み済みなら早期return
		return;
	}
	// モデルの生成とファイル読み込み、初期化
	std::unique_ptr<AnimationModel> model = std::make_unique<AnimationModel>();
	model->Initialize(directoryPath, filePath,
		{ { 1.0f,1.0f,1.0f,1.0 },true
		});
	animationModels_.insert(std::make_pair(filePath, std::move(model)));
}

Model* ModelManager::FindModel(const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (models_.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return models_.at(filePath).get();
	}

	// ファイル名一致無し
	return nullptr;
}

AnimationModel* ModelManager::FindAnimationModel(const std::string& filePath)
{
	// 読み込み済みモデルを検索
	if (animationModels_.contains(filePath)) {
		// 読み込みモデルを戻り値としてreturn
		return animationModels_.at(filePath).get();
	}

	// ファイル名一致無し
	return nullptr;
}

ModelManager* ModelManager::GetInstance() {
	static ModelManager instance;
	return &instance;
}
