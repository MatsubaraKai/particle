#pragma once
#include <d3d12.h>
#include "DirectXCommon.h"
#include "PSOProperty.h"

/**
 * @file PSO.h
 * @brief パイプラインステートオブジェクト（PSO）を管理するクラス
 */

 /// <summary>
 /// パイプラインステートオブジェクト（PSO）を管理するクラス
 /// </summary>
class PSO : public PSOProperty
{
public:
    /// <summary>
    /// シングルトンのインスタンスを取得する
    /// </summary>
    /// <returns>PSOクラスのインスタンス</returns>
    static PSO* GetInstance();

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PSO() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~PSO() = default;

    /// <summary>
    /// コピー代入演算子を削除（コピー不可）
    /// </summary>
    /// <param name="other">コピー元のオブジェクト</param>
    /// <returns>コピー不可</returns>
    const PSO& operator=(const PSO&) = delete;

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
    /// PSOプロパティを取得する
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
    D3D12_ROOT_PARAMETER rootParameters[6] = {};

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
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

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
