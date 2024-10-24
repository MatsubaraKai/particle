#include "GaussianBlur.h"
#include <function.h>
#include <DirectXCommon.h>
#include "PostProcess.h"
#include "SRVManager.h"
#include "Mesh.h"
/**
* @file GaussianBlur.cpp
* @brief GaussianBlur
*/
void GaussianBlur::Init()
{
	// 実際に頂点リソースを作る
	depthOutlineResource_ = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(GaussianBlur));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	depthOutlinelData_ = nullptr;
	// 書き込むためのアドレスを取得
	depthOutlineResource_->Map(0, nullptr, reinterpret_cast<void**>(&depthOutlinelData_));

}

PSOProperty GaussianBlur::CreatePipelineStateObject()
{
	PSOProperty property;
	HRESULT hr;
	// DirectXCommonのインスタンスを取得
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	//std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange = CreateDescriptorRange();


	/*---------------------*/
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange = CreateDescriptorRange();

	std::vector<D3D12_ROOT_PARAMETER> rootParameters = CreateRootParamerter(descriptorRange);

	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers = CreateSampler();

	// 上記を元にRootSignatureを作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature = CreateRootSignature(property, rootParameters, staticSamplers);

	D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc = SetInputLayout();

	// blendStateの設定
	D3D12_BLEND_DESC blendDesc = SetBlendState();

	D3D12_RASTERIZER_DESC rasterizerDesc = SetRasterizerState();

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc = CreateDepth();

	// Shaderをコンパイルする
	property.vertexShaderBlob = CompileShader(L"Resources/shader/Fullscreen.VS.hlsl",
		L"vs_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
	assert(property.vertexShaderBlob != nullptr);

	property.pixelShaderBlob = CompileShader(L"Resources/shader/GaussianFilter.PS.hlsl",
		L"ps_6_0", sDirectXCommon->GetDxcUtils(), sDirectXCommon->GetDxcCompiler(), sDirectXCommon->GetIncludeHandler());
	assert(property.pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
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
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//実際に生成
	property.graphicsPipelineState = nullptr;
	hr = sDirectXCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&property.graphicsPipelineState));
	assert(SUCCEEDED(hr));
	return property;
}


void GaussianBlur::CommandRootParameter(PostProcess* postProcess)
{
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	depthOutlinelData_->deviation = postProcess->GetDeviation();
	// マテリアルCBufferの場所を設定
	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(0, SRVManager::GetInstance()->GetGPUDescriptorHandle(sDirectXCommon->GetRenderIndex()));

	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetGPUDescriptorHandle(sDirectXCommon->GetDepthIndex()));

	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetGPUDescriptorHandle(postProcess->GetNoisetex()));
	// マテリアルCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, depthOutlineResource_->GetGPUVirtualAddress());
}

std::vector<D3D12_DESCRIPTOR_RANGE> GaussianBlur::CreateDescriptorRange()
{
	std::vector<D3D12_DESCRIPTOR_RANGE> descriptorRange(3);
	descriptorRange[0].BaseShaderRegister = 0; // 0から始まる
	descriptorRange[0].NumDescriptors = 1; // 数は2つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	descriptorRange[1].BaseShaderRegister = 1; // 0から始まる
	descriptorRange[1].NumDescriptors = 1; // 数は2つ
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	descriptorRange[2].BaseShaderRegister = 2; // 0から始まる
	descriptorRange[2].NumDescriptors = 1; // 数は2つ
	descriptorRange[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[2].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算
	return descriptorRange;
}

std::vector<D3D12_ROOT_PARAMETER> GaussianBlur::CreateRootParamerter(std::vector<D3D12_DESCRIPTOR_RANGE>& descriptorRange)
{


	std::vector<D3D12_ROOT_PARAMETER> rootParameters(4);
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = &descriptorRange[0]; // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[1].DescriptorTable.pDescriptorRanges = &descriptorRange[1]; // Tableの中身の配列を指定
	rootParameters[1].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange[2]; // Tableの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = 1; // Tableで利用する数

	// ProjectionInverseを送る用　Matria
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;  // PixelShaderで使う
	rootParameters[3].Descriptor.ShaderRegister = 0; //レジスタ番号0とバインド
	return rootParameters;
}

std::vector<D3D12_STATIC_SAMPLER_DESC> GaussianBlur::CreateSampler()
{
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplers(2);
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

	return staticSamplers;
}

D3D12_ROOT_SIGNATURE_DESC GaussianBlur::CreateRootSignature(PSOProperty& pso, std::vector<D3D12_ROOT_PARAMETER>& rootParameters, std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers)
{
	HRESULT hr;
	// RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootSignature.pParameters = rootParameters.data(); // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = static_cast<UINT>(rootParameters.size()); // 配列の長さ
	descriptionRootSignature.pStaticSamplers = samplers.data();
	descriptionRootSignature.NumStaticSamplers = static_cast<UINT>(samplers.size());

	pso.errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &pso.signatureBlob, &pso.errorBlob);
	if (FAILED(hr)) {
		ConsoleLog(reinterpret_cast<char*>(pso.errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	pso.rootSignature = nullptr;
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateRootSignature(0, pso.signatureBlob->GetBufferPointer(),
		pso.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pso.rootSignature));
	assert(SUCCEEDED(hr));
	return descriptionRootSignature;
}

D3D12_INPUT_LAYOUT_DESC GaussianBlur::SetInputLayout()
{
	// 頂点には何もデータを入力しないので、InputLayoutは利用しない。ドライバやGPUのやることが
	// 少なくなりそうな気配を感じる
	D3D12_INPUT_LAYOUT_DESC  inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;
	return inputLayoutDesc;
}

D3D12_BLEND_DESC GaussianBlur::SetBlendState()
{
	// blendStateの設定
	//すべての色要素を書き込む
	// blendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	return blendDesc;
}

D3D12_RASTERIZER_DESC GaussianBlur::SetRasterizerState()
{
	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面（時計回り）を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	return rasterizerDesc;
}

D3D12_DEPTH_STENCIL_DESC GaussianBlur::CreateDepth()
{
	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// 全画面に対してなにか処理を施したいだけだから、比較も書き込みも必要ないのでDepth自体不要
	depthStencilDesc.DepthEnable = false;
	return depthStencilDesc;
}

