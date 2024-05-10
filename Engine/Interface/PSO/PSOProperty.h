#pragma once
#include <d3d12.h>
#include "DirectXCommon.h"


enum BlendMode {
	//!< ブレンドなし	
	kBlendModeNone,
	//!< 通常αブレンド。 デフォルト。 Src * SrcA + Dest * (1 - SrcA)
	kBlendModeNormal,
	//!< 加算。 Src * SrcA + Dest * 1
	kBlendModeAdd,
	//!< 減算。 Dest * 1 - Src * SrcA
	kBlendModeSubtract,
	//!< 乗算。Src * (1 - Dest) + Dest* 1
	kBlendModeMultiply,
	//!< スクリーン。 Src * (1 - Dest) + Dest + 1
	kBlendModeScreen,
	// 利用してはいけない
	kCountOfBlendMode,
};

class PSOProperty {
public:
	struct  {
		Microsoft::WRL::ComPtr < ID3D12PipelineState> graphicsPipelineState;
		Microsoft::WRL::ComPtr < ID3D12RootSignature> rootSignature;
		Microsoft::WRL::ComPtr < ID3DBlob> signatureBlob;
		Microsoft::WRL::ComPtr < ID3DBlob> errorBlob;
		Microsoft::WRL::ComPtr < IDxcBlob> vertexShaderBlob;
		Microsoft::WRL::ComPtr < IDxcBlob> pixelShaderBlob;
	};
protected:

	static int BlendNo_;
};