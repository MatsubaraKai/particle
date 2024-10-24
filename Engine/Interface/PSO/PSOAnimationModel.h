#pragma once
#include <d3d12.h>
#include "DirectXCommon.h"
#include "PSOProperty.h"
#include "Vector3.h"

/**
 * @file PSOAnimationModel.h
 * @brief アニメーションモデル用のパイプラインステートオブジェクトを管理するクラス
 */

 /// <summary>
 /// GPU用のカメラ情報を保持する構造体
 /// </summary>
struct CameraForGPU {
    /// <summary>
    /// カメラのワールド座標
    /// </summary>
    Vector3 worldPosition;
};

/// <summary>
/// アニメーションモデルのパイプラインステートオブジェクトを管理するクラス
/// </summary>
class PSOAnimationModel : public PSOProperty
{
public:
    /// <summary>
    /// シングルトンインスタンスを取得する関数
    /// </summary>
    /// <returns>PSOAnimationModelのインスタンス</returns>
    static PSOAnimationModel* GetInstance();

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PSOAnimationModel() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~PSOAnimationModel() = default;

    /// <summary>
    /// コピー代入演算子を削除（コピー不可）
    /// </summary>
    /// <param name="other">コピー元のオブジェクト</param>
    /// <returns>コピー不可</returns>
    const PSOAnimationModel& operator=(const PSOAnimationModel&) = delete;

    /// <summary>
    /// 描画に関する設定をまとめる関数
    /// </summary>
    void CreatePipelineStateObject();

    /// <summary>
    /// シェーダーとリソースの関係を設定する関数
    /// </summary>
    void CreateRootSignature();

    /// <summary>
    /// 頂点シェーダーへ渡す頂点データの指定を行う関数
    /// </summary>
    void SetInputLayout();

    /// <summary>
    /// ピクセルシェーダーからの出力をどのように書き込むか設定する関数
    /// </summary>
    void SetBlendState();

    /// <summary>
    /// ラスタライザーに関する設定を行う関数
    /// </summary>
    void SetRasterizerState();

    /// <summary>
    /// デプスバッファを生成する関数
    /// </summary>
    void CreateDepth();

    /// <summary>
    /// PSOプロパティを取得する関数
    /// </summary>
    /// <returns>PSOPropertyオブジェクト</returns>
    PSOProperty GetProperty() { return property; }

private:
    HRESULT hr_;  ///< 処理結果を格納するHRESULT

    /// <summary>
    /// ルートシグネチャの記述子
    /// </summary>
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

    /// <summary>
    /// ルートパラメータの配列
    /// </summary>
    D3D12_ROOT_PARAMETER rootParameters[7] = {};

    /// <summary>
    /// スタティックサンプラーディスクリプタ
    /// </summary>
    D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

    /// <summary>
    /// ディスクリプタレンジの配列
    /// </summary>
    D3D12_DESCRIPTOR_RANGE descriptorRange_[2] = {};

    /// <summary>
    /// 入力レイアウトの要素記述子
    /// </summary>
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[5] = {};

    /// <summary>
    /// 入力レイアウトの記述子
    /// </summary>
    D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

    /// <summary>
    /// ブレンドステートの設定
    /// </summary>
    D3D12_BLEND_DESC blendDesc{};

    /// <summary>
    /// ラスタライザーステートの設定
    /// </summary>
    D3D12_RASTERIZER_DESC rasterizerDesc{};

    /// <summary>
    /// グラフィックスパイプラインステートの記述子
    /// </summary>
    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

    /// <summary>
    /// デプスステンシルステートの設定
    /// </summary>
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

    /// <summary>
    /// 描画先のDSVを設定するためのハンドル
    /// </summary>
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

    /// <summary>
    /// PSOプロパティ
    /// </summary>
    PSOProperty property;
};
