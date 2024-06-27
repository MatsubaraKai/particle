#include "PSOPostEffect.h"
#include <d3dx12.h>


void PSOPostEffect::CreatePipelineStateObject() {
	// DirectXCommonのインスタンスを取得
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();

	PSOPostEffect::CreateRootSignature();
	PSOPostEffect::SetInputLayout();
	PSOPostEffect::SetBlendState();
	PSOPostEffect::SetRasterrizerState();
	PSOPostEffect::CreateDepth();
	// Shaderをコンパイルする
	property.vertexShaderBlob = CompileShader(L"Resources/shader/Fullscreen.VS.hlsl",
		L"vs_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
	assert(property.vertexShaderBlob != nullptr);

	property.pixelShaderBlob = CompileShader(L"Resources/shader/LuminanceBasedOutline.PS.hlsl",
		L"ps_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
	assert(property.pixelShaderBlob != nullptr);

	graphicsPipelineStateDesc.pRootSignature = property.rootSignature.Get(); // RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { property.vertexShaderBlob->GetBufferPointer(),
	property.vertexShaderBlob->GetBufferSize() };//vertexShader
	graphicsPipelineStateDesc.PS = { property.pixelShaderBlob->GetBufferPointer(),
	property.pixelShaderBlob->GetBufferSize() };// PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc;//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; //ReterizerState
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ（形状）のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくてよい）
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DeptjStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc_;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	property.graphicsPipelineState = nullptr;
	hr_ = sDirectXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&property.graphicsPipelineState));
	assert(SUCCEEDED(hr_));

	//DirectXCommon::GetInstance()->ChangeDepthStatetoRender();
}

void PSOPostEffect::CreateRootSignature() {
	// DirectXCommonのインスタンスを取得
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// シリアライズしてバイナリにする
	property.signatureBlob = nullptr;



	descriptorRange_[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange_[0].NumDescriptors = 1; // 数は2つ
	descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	descriptorRange_[1].BaseShaderRegister = 1; // 0から始まる
	descriptorRange_[1].NumDescriptors = 1; // 数は2つ
	descriptorRange_[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange_[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	descriptorRange_[2].BaseShaderRegister = 2; // 0から始まる
	descriptorRange_[2].NumDescriptors = 1; // 数は2つ
	descriptorRange_[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange_[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算


	rootParamerters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParamerters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParamerters[0].DescriptorTable.pDescriptorRanges = &descriptorRange_[0]; // Tableの中身の配列を指定
	rootParamerters[0].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	rootParamerters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParamerters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParamerters[1].DescriptorTable.pDescriptorRanges = &descriptorRange_[1]; // Tableの中身の配列を指定
	rootParamerters[1].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	rootParamerters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParamerters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParamerters[2].DescriptorTable.pDescriptorRanges = &descriptorRange_[2]; // Tableの中身の配列を指定
	rootParamerters[2].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	// ProjectionInverseを送る用　Matria
	rootParamerters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParamerters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  // PixelShaderで使う
	rootParamerters[3].Descriptor.ShaderRegister = 0; //レジスタ番号0とバインド


	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイナリフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; // バイナリフィルタ
	staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; // 0~1の範囲外をリピート
	staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[1].ShaderRegister = 1; // レジスタ番号0を使う
	staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	descriptionRootSignature.pParameters = rootParamerters; // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParamerters); // 配列の長さ
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	property.errorBlob = nullptr;
	hr_ = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &property.signatureBlob, &property.errorBlob);
	if (FAILED(hr_)) {
		ConsoleLog(reinterpret_cast<char*>(property.errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	property.rootSignature = nullptr;
	hr_ = sDirectXCommon->GetDevice()->CreateRootSignature(0, property.signatureBlob->GetBufferPointer(),
		property.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&property.rootSignature));
	assert(SUCCEEDED(hr_));
}

void PSOPostEffect::SetInputLayout() {
	// 頂点には何もデータを入力しないので、InputLayoutは利用しない。ドライバやGPUのやることが
	// 少なくなりそうな気配を感じる
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

}

void PSOPostEffect::SetBlendState() {
	// blendStateの設定
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}

void PSOPostEffect::SetRasterrizerState() {
	//裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
}

void PSOPostEffect::CreateDepth()
{
	// 全画面に対してなにか処理を施したいだけだから、比較も書き込みも必要ないのでDepth自体不要
	depthStencilDesc_.DepthEnable = false;

}


PSOPostEffect* PSOPostEffect::GatInstance() {
	static PSOPostEffect instance;
	return &instance;
}