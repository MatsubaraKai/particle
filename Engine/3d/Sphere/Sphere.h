#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>

#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "DirectionLight.h"
#include "Mesh.h"
#include "PSOModel.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

/**
* @file Sphere.h
* @brief 球体モデルを描画するクラス
*/

class DirectXCommon;
class WinAPI;
class TextureManager;
class Camera;

class Sphere
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Sphere();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Sphere();

    /// <summary>
    /// 初期化処理を行う
    /// </summary>
    /// <param name="camera">カメラオブジェクトへのポインタ</param>
    void Initialize(Camera* camera);

    /// <summary>
    /// 更新処理を行う
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理を行う
    /// </summary>
    /// <param name="transform">トランスフォームデータ</param>
    /// <param name="texture">テクスチャハンドル</param>
    void Draw(Transform transform, uint32_t texture);

    /// <summary>
    /// テクスチャマネージャーを設定する
    /// </summary>
    /// <param name="textureManager">テクスチャマネージャーのポインタ</param>
    void SetTextureManager(TextureManager* textureManager) {
        textureManager_ = textureManager;
    }

    /// <summary>
    /// 球体を描画する
    /// </summary>
    /// <param name="sphere">描画する球体の情報</param>
    /// <param name="viewProjectionMatrix">ビュー・プロジェクション行列</param>
    /// <param name="viewportMatrix">ビューポート行列</param>
    /// <param name="color">描画色</param>
    void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color);

    /// <summary>
    /// 頂点バッファビューを作成する
    /// </summary>
    /// <returns>作成された頂点バッファビュー</returns>
    D3D12_VERTEX_BUFFER_VIEW CreateBufferView();

    /// <summary>
    /// 平行光源のデータへのポインタ
    /// </summary>
    DirectionalLight* directionalLightData;

    /// <summary>
    /// UV変換用のトランスフォーム
    /// </summary>
    Transform transformUv;

private:
    /// <summary>
    /// パイプラインステートオブジェクト
    /// </summary>
    PSO* pso_ = nullptr;

    /// <summary>
    /// WinAPIオブジェクトへのポインタ
    /// </summary>
    WinAPI* sWinAPI = nullptr;

    /// <summary>
    /// DirectX共通オブジェクトへのポインタ
    /// </summary>
    DirectXCommon* sDirectXCommon_ = nullptr;

    /// <summary>
    /// テクスチャマネージャー
    /// </summary>
    TextureManager* textureManager_ = nullptr;

    /* 頂点用 */
    /// <summary>
    /// 頂点リソース（GPUメモリ上の頂点バッファ）
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

    /// <summary>
    /// 頂点バッファビュー（描画時に使用するビュー）
    /// </summary>
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

    /// <summary>
    /// 頂点データへのポインタ
    /// </summary>
    VertexData* vertexData_;

    /* 色用 */
    /// <summary>
    /// マテリアルリソース（GPUメモリ上のマテリアルデータ）
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

    /// <summary>
    /// マテリアルバッファビュー
    /// </summary>
    D3D12_VERTEX_BUFFER_VIEW materialBufferView{};

    /// <summary>
    /// マテリアルデータへのポインタ
    /// </summary>
    Material* materialData;

    /// <summary>
    /// 平行光源用のリソース
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource;

    /* 移動用 */
    /// <summary>
    /// WVP（ワールド・ビュー・プロジェクション）用のリソース
    /// </summary>
    Microsoft::WRL::ComPtr<ID3D12Resource> wvpResource;

    /// <summary>
    /// WVPデータへのポインタ
    /// </summary>
    TransformationMatrix* wvpData;

    /// <summary>
    /// WVPの頂点バッファビュー
    /// </summary>
    D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};

    /// <summary>
    /// スタティックサンプラーディスクリプタ
    /// </summary>
    D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

    /// <summary>
    /// ディスクリプタレンジ
    /// </summary>
    D3D12_DESCRIPTOR_RANGE descriptorRange_[1] = {};

    /// <summary>
    /// ビューポート設定
    /// </summary>
    D3D12_VIEWPORT viewport{};

    /// <summary>
    /// シザー矩形
    /// </summary>
    D3D12_RECT scissorRect{};

    /// <summary>
    /// カメラオブジェクトへのポインタ
    /// </summary>
    Camera* camera_ = nullptr;
};
