#pragma once
#include "IPostEffectState.h"
#include "PSOProperty.h"
#include "Matrix4x4.h"
/**
* @file ChromaticAberration.h
* @brief ChromaticAberration
*/
struct ChromaticAberrationInfo {

};
class PostProcess;
class ChromaticAberration : public IPostEffectState
{
public:
	void Init() override;
	/// <summary>
	/// 描画に関する設定をまとめる関数
	/// </summary>
	PSOProperty CreatePipelineStateObject() override;

	/// <summary>
	/// 設定したことを元にコマンドリストを使う
	/// </summary>
	void CommandRootParameter(PostProcess* postProcess) override;



	std::vector<D3D12_DESCRIPTOR_RANGE> CreateDescriptorRange() override;

	/// <summary>
	/// Shaderに渡す値を決める
	/// </summary>
	/// <returns></returns>
	std::vector<D3D12_ROOT_PARAMETER> CreateRootParamerter(std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRange) override;

	/// <summary>
	/// PSにの書き込み方の設定
	/// </summary>
	/// <returns></returns>
	std::vector<D3D12_STATIC_SAMPLER_DESC> CreateSampler() override;

	/// <summary>
	/// ShaderとResourceを関係のつけ方を決める関数
	/// </summary>
	D3D12_ROOT_SIGNATURE_DESC CreateRootSignature(PSOProperty& pso, std::vector<D3D12_ROOT_PARAMETER>& rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers) override;

	/// <summary>
	/// VSへ渡す頂点データの指定を行う関数
	/// </summary>
	D3D12_INPUT_LAYOUT_DESC SetInputLayout() override;

	/// <summary>
	/// PSからの出力をどう書き込むかの設定を行う関数
	/// </summary>
	D3D12_BLEND_DESC SetBlendState() override;

	/// <summary>
	/// ラスタライザーに関する設定の関数
	/// </summary>
	D3D12_RASTERIZER_DESC SetRasterizerState() override;

	/// <summary>
	/// DepthBufferの生成
	/// </summary>
	D3D12_DEPTH_STENCIL_DESC CreateDepth() override;
private:
	//PostProcess* postProcess_ = nullptr;
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr <ID3D12Resource> depthOutlineResource_;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む
};

