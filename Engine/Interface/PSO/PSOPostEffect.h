#pragma once
#pragma once
#include <d3d12.h>
#include "DirectXCommon.h"
#include "PSOProperty.h"
#include "Vector3.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>


class PSOPostEffect : public PSOProperty
{
public:
	static PSOPostEffect* GatInstance();

	PSOPostEffect() = default;
	~PSOPostEffect() = default;
	const PSOPostEffect& operator=(const PSOPostEffect&) = delete;


	/// <summary>
	/// 描画に関する設定をまとめる関数
	/// </summary>
	void CreatePipelineStateObject();

	/// <summary>
	/// ShaderとResourceを関係のつけ方を決める関数
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// VSへ渡す頂点データの指定を行う関数
	/// </summary>
	void SetInputLayout();

	/// <summary>
	/// PSからの出力をどう書き込むかの設定を行う関数
	/// </summary>
	void SetBlendState();

	/// <summary>
	/// ラスタライザーに関する設定の関数
	/// </summary>
	void SetRasterrizerState();

	/// <summary>
	/// Release
	/// </summary>
	/// <returns></returns>

	/// <summary>
	/// DepthBufferの生成
	/// </summary>
	void CreateDepth();


	PSOProperty GetProperty() { return property; }

private:
	HRESULT hr_;
	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	// シリアライズしてバイナリにする
	//ID3DBlob* signatureBlob;
	//ID3DBlob* errorBlob;
	// バイナリを元に生成
	//ID3D12RootSignature* rootSignature;
	// RootParmeter作成。複数でっていできるので配列。今回は結果１つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER rootParamerters[4] = {};

	D3D12_STATIC_SAMPLER_DESC staticSamplers[2] = {};

	D3D12_DESCRIPTOR_RANGE descriptorRange_[3] = {};

	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc{};

	// blendStateの設定
	D3D12_BLEND_DESC blendDesc{};

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};

	/*IDxcBlob* vertexShaderBlob;
	IDxcBlob* pixelShaderBlob;*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	//実際に生成
	//ID3D12PipelineState* graphicsPipelineState;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};

	// 描画先のRTVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	PSOProperty property;

};



