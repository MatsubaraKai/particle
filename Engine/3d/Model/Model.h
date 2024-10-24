#pragma once
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "PSOModel.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "Transform.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "vertexData.h"
#include "ModelData.h"
#include "MaterialData.h"
#include "mathFunction.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "Animation.h"

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

/**
* @file Model.h
* @brief 3Dモデルの管理および描画を行うクラス
*/
class Model
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Model();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Model();

    /// <summary>
    /// モデルデータを初期化する関数
    /// </summary>
    /// <param name="directoryPath">モデルファイルのディレクトリパス</param>
    /// <param name="filePath">モデルファイルのファイルパス</param>
    /// <param name="material">マテリアルデータ</param>
    void Initialize(const std::string& directoryPath, const std::string& filePath, const Material& material);

    /// <summary>
    /// 更新処理を行う関数
    /// </summary>
    void Update();

    /// <summary>
    /// 描画を行う関数
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="material">マテリアル</param>
    /// <param name="dire">平行光源データ</param>
    /// <param name="mapTexture">マップテクスチャ</param>
    void Draw(uint32_t texture, const Material& material, const DirectionalLight& dire, uint32_t mapTexture);

    /// <summary>
    /// テクスチャマネージャーをセットする関数
    /// </summary>
    void SetTextureManager(TextureManager* textureManager) {
        textureManager_ = textureManager;
    }

    /// <summary>
    /// OBJファイルを読み込む関数
    /// </summary>
    ModelData LoadObjFile(const std::string& directoryPath, const std::string& filePath);

    /// <summary>
    /// GLTFファイルを読み込む関数
    /// </summary>
    ModelData LoadGLTFFile(const std::string& directoryPath, const std::string& filePath);

    /// <summary>
    /// マテリアルテンプレートファイルを読み込む関数
    /// </summary>
    MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

    /// <summary>
    /// アニメーション行列を取得する関数
    /// </summary>
    /// <returns>アニメーション行列</returns>
    Matrix4x4 GetAniMatrix() { return aniMatrix_; }

    /// <summary>
    /// スケルトン行列を取得する関数
    /// </summary>
    /// <returns>スケルトン行列</returns>
    Matrix4x4 GetSkeMatrix() { return skeMatrix_; }

    /// <summary>
    /// アニメーションファイルを読み込む関数
    /// </summary>
    AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filePath);

    /// <summary>
    /// ノードデータを読み取る関数
    /// </summary>
    /// <param name="node">Assimpのノード</param>
    /// <returns>読み取ったノードデータ</returns>
    Node ReadNode(aiNode* node);

    /// <summary>
    /// アニメーションを適用する関数
    /// </summary>
    /// <param name="skeleton">スケルトンデータ</param>
    /// <param name="animation">アニメーションデータ</param>
    /// <param name="animationTime">アニメーションの再生時間</param>
    void ApplyAnimation(SkeletonData& skeleton, const AnimationData& animation, float animationTime);

    /// <summary>
    /// モデルデータを取得する
    /// </summary>
    /// <returns>モデルデータ</returns>
    ModelData GetModelData() { return modelData_; }

private:
    HRESULT hr;  ///< 処理結果確認用変数

    Material* materialData;  ///< マテリアルデータ

    VertexData* vertexData_;  ///< 頂点データ
    ModelData modelData_;  ///< モデルデータ
    AnimationData animation_;  ///< アニメーションデータ
    SkeletonData skeleton_;  ///< スケルトンデータ
    SkinCluster skinCluster_;  ///< スキンクラスターデータ

    DirectXCommon* directXCommon_;  ///< DirectXの共通処理
    WinAPI* sWinAPI_;  ///< Windows API
    TextureManager* textureManager_ = nullptr;  ///< テクスチャマネージャー
    PSO* pso_ = nullptr;  ///< パイプラインステートオブジェクト

    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;  ///< 頂点リソース
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;  ///< 頂点バッファビュー
    D3D12_VERTEX_BUFFER_VIEW vbvs[2]{};  ///< 複数頂点バッファビュー

    Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;  ///< マテリアルリソース
    D3D12_VERTEX_BUFFER_VIEW materialBufferView{};  ///< マテリアルバッファビュー

    Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;  ///< 平行光源リソース
    Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;  ///< インデックスリソース
    D3D12_INDEX_BUFFER_VIEW indexBufferView_{};  ///< インデックスバッファビュー

    DirectionalLight* directionalLightData;  ///< 平行光源データ
    Transform transformUv;  ///< UV座標の変換情報
    D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};  ///< ワールド・ビュー・プロジェクションバッファビュー

    Camera* camera_ = nullptr;  ///< カメラデータ

    Matrix4x4 aniMatrix_;  ///< アニメーション行列
    Matrix4x4 skeMatrix_;  ///< スケルトン行列
    float animationTime = 0.0f;  ///< アニメーション時間
};
