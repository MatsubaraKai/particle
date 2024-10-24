#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include <string>
#include <vector>
#include <optional>
#include <map>
#include "ModelData.h"
#include <span>
#include <array>
#include <wrl.h>
#include <d3d12.h>

/**
* @file Skeleton.h
* @brief スケルトンやアニメーションデータを管理するクラスおよび構造体
*/

/// <summary>
/// オイラー角による変換を表す構造体
/// </summary>
struct EulerTransform {
    Vector3 Tscale;  ///< スケール
    Vector3 rotate;  ///< オイラー角での回転
    Vector3 translate;  ///< 平行移動
};

/// <summary>
/// クォータニオンによる変換を表す構造体
/// </summary>
struct QuaternionTransform {
    Vector3 scale;       ///< スケール
    Quaternion rotate;   ///< 回転（クォータニオン）
    Vector3 translate;   ///< 平行移動
};

/// <summary>
/// ジョイントを表す構造体
/// </summary>
struct Joint {
    QuaternionTransform transform;  ///< 変換情報
    Matrix4x4 localMatrix;          ///< ローカル行列
    Matrix4x4 skeletonSpaceMatrix;  ///< スケルトン空間での変換行列
    std::string name;               ///< ジョイントの名前
    std::vector<int32_t> children;  ///< 子ジョイントのインデックスリスト（子がいなければ空）
    int32_t index;                  ///< 自身のインデックス
    std::optional<int32_t> parent;  ///< 親ジョイントのインデックス（親がいなければnull）
};                         

/// <summary>
/// スケルトンデータを表す構造体
/// </summary>
struct SkeletonData {
    int32_t root;  ///< ルートジョイントのインデックス
    std::map<std::string, int32_t> jointMap;  ///< ジョイント名とインデックスのマップ
    std::vector<Joint> joints;  ///< ジョイントのリスト
};

/// <summary>
/// ノードを表す構造体
/// </summary>
struct Node {
    QuaternionTransform transform;  ///< 変換情報
    Matrix4x4 localMatrix;          ///< ローカル行列
    std::string name;               ///< ノードの名前
    std::vector<Node> children;     ///< 子ノードのリスト
};

/// <summary>
/// 頂点のウェイトデータを表す構造体
/// </summary>
struct VertexWeightData {
    float weight;          ///< ウェイト値
    uint32_t vertexIndex;  ///< 対応する頂点のインデックス
};

/// <summary>
/// ジョイントのウェイトデータを表す構造体
/// </summary>
struct JointWeightData {
    Matrix4x4 inverseBindPoseMatrix;          ///< インバースバインドポーズ行列
    std::vector<VertexWeightData> vertexWeights;  ///< 頂点ウェイトのリスト
};

/// <summary>
/// モデルデータを表す構造体
/// </summary>
struct ModelData {
    std::map<std::string, JointWeightData> skinClusterData;  ///< スキンクラスターデータのマップ
    std::vector<VertexData> vertices;  ///< 頂点データのリスト
    std::vector<uint32_t> indices;     ///< インデックスデータのリスト
    MaterialData material;             ///< マテリアルデータ
    Node rootNode;                     ///< ルートノード
};

const uint32_t kNumMaxInfluence = 4;

/// <summary>
/// 頂点に影響を与えるジョイント情報を表す構造体
/// </summary>
struct VertexInfluence {
    std::array<float, kNumMaxInfluence> weights;       ///< ウェイト値の配列
    std::array<int32_t, kNumMaxInfluence> jointIndices;  ///< ジョイントインデックスの配列
};

/// <summary>
/// GPU用のウェルデータを表す構造体
/// </summary>
struct WellForGPU {
    Matrix4x4 skeletonSpaceMatrix;                ///< スケルトン空間での変換行列（位置用）
    Matrix4x4 skeletonSpaceInverseTransposeMatrix;  ///< スケルトン空間での逆転置行列（法線用）
};

/// <summary>
/// スキンクラスターデータを表す構造体
/// </summary>
struct SkinCluster {
    std::vector<Matrix4x4> inverseBindPoseMatrices;  ///< インバースバインドポーズ行列のリスト
    Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;  ///< インフルエンスリソース
    D3D12_VERTEX_BUFFER_VIEW influenceBufferView;               ///< インフルエンスバッファビュー
    std::span<VertexInfluence> mappedInfluence;                 ///< マップされたインフルエンスデータ
    Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;     ///< パレットリソース
    std::span<WellForGPU> mappedPalette;                        ///< マップされたパレットデータ
    std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;  ///< パレットのSRVハンドル
};

/// <summary>
/// スケルトンを管理するクラス
/// </summary>
class Skeleton {
public:
    /// <summary>
    /// スケルトンの更新を行う
    /// </summary>
    /// <param name="skeleton">更新するスケルトンデータ</param>
    void Update(SkeletonData& skeleton);

    /// <summary>
    /// ジョイントを作成する静的関数
    /// </summary>
    /// <param name="node">ノードデータ</param>
    /// <param name="parent">親ジョイントのインデックス（ない場合はnull）</param>
    /// <param name="joints">ジョイントのリスト</param>
    /// <returns>作成されたジョイントのインデックス</returns>
    static int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

    /// <summary>
    /// スケルトンを作成する静的関数
    /// </summary>
    /// <param name="rootNode">ルートノードデータ</param>
    /// <returns>作成されたスケルトンデータ</returns>
    static SkeletonData CreateSkeleton(const Node& rootNode);

    /// <summary>
    /// スキンクラスタを作成する静的関数
    /// </summary>
    /// <param name="device">DirectXデバイス</param>
    /// <param name="skeleton">スケルトンデータ</param>
    /// <param name="modelData">モデルデータ</param>
    /// <param name="descriptorHeap">デスクリプタヒープ</param>
    /// <param name="descriptorSize">デスクリプタのサイズ</param>
    /// <returns>作成されたスキンクラスターデータ</returns>
    static SkinCluster CreateSkinCluster(
        const Microsoft::WRL::ComPtr<ID3D12Device>& device,
        const SkeletonData& skeleton,
        const ModelData& modelData,
        const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptorHeap,
        uint32_t descriptorSize
    );

private:
    // プライベートメンバ（必要に応じて追加）
};
