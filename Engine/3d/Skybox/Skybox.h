#pragma once
#include <wrl.h>
#include <vector>

#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <d3d12.h>
#include <Transform.h>
#include <Camera.h>
#include <Material.h>
#include "DirectionLight.h"

/**
* @file Skybox.h
* @brief スカイボックスを描画するためのクラス
*/
class Skybox
{
public:
    /// <summary>
    /// スカイボックスの初期化を行う
    /// </summary>
    /// <param name="material">使用するマテリアル</param>
    void Init(const Material& material);

    /// <summary>
    /// スカイボックスの描画を行う
    /// </summary>
    /// <param name="texture">テクスチャのハンドル</param>
    /// <param name="material">使用するマテリアル</param>
    /// <param name="dire">平行光源の情報</param>
    void Draw(uint32_t texture, const Material& material, const DirectionalLight& dire);

private:
    /// <summary>
    /// 頂点リソース（GPUメモリ上の頂点バッファ）
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

    /// <summary>
    /// 頂点バッファビュー（描画時に使用するビュー）
    /// </summary>
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;

    /// <summary>
    /// 頂点データへのポインタ
    /// </summary>
    VertexData* vertexData_;

    /// <summary>
    /// マテリアルリソース（GPUメモリ上のマテリアルデータ）
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

    /// <summary>
    /// マテリアルバッファビュー
    /// </summary>
    D3D12_VERTEX_BUFFER_VIEW materialBufferView{};

    /// <summary>
    /// 平行光源用のリソース
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;

    /// <summary>
    /// インデックスバッファ用のデータ
    /// </summary>
    uint32_t* indexDataSprite;

    /// <summary>
    /// インデックスバッファリソース
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;

    /// <summary>
    /// インデックスバッファビュー
    /// </summary>
    D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

    /// <summary>
    /// 平行光源データへのポインタ
    /// </summary>
    DirectionalLight* directionalLightData;

    /// <summary>
    /// モデルデータ
    /// </summary>
    ModelData modelData_;

    /// <summary>
    /// マテリアルデータへのポインタ
    /// </summary>
    Material* materialData_;

    /// <summary>
    /// UV変換用のトランスフォーム
    /// </summary>
    Transform transformUv;

    /// <summary>
    /// WVP（ワールド・ビュー・プロジェクション）バッファビュー
    /// </summary>
    D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};

    /// <summary>
    /// カメラオブジェクトへのポインタ
    /// </summary>
    Camera* camera_ = nullptr;

    /// <summary>
    /// アニメーション用の行列
    /// </summary>
    Matrix4x4 aniMatrix_;

    /// <summary>
    /// スケルトン行列
    /// </summary>
    Matrix4x4 skeMatrix_;

    /// <summary>
    /// アニメーションの経過時間
    /// </summary>
    float animationTime = 0.0f;
};
