#include "PSOAnimationModel.h"

/**
 * @file PSOAnimationModel.cpp
 * @brief アニメーションモデル用のパイプラインステートオブジェクトを管理するクラスの実装
 */

 /// <summary>
 /// 描画に関する設定をまとめる関数
 /// </summary>
void PSOAnimationModel::CreatePipelineStateObject() {
    // DirectXCommonのインスタンスを取得
    DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();

    // ルートシグネチャの作成
    PSOAnimationModel::CreateRootSignature();
    // 入力レイアウトの設定
    PSOAnimationModel::SetInputLayout();
    // ブレンドステートの設定
    PSOAnimationModel::SetBlendState();
    // ラスタライザーステートの設定
    PSOAnimationModel::SetRasterizerState();
    // デプスステンシルステートの設定
    PSOAnimationModel::CreateDepth();

    // シェーダーをコンパイルする
    property.vertexShaderBlob = CompileShader(
        L"Resources/shader/SkinningObject3d.VS.hlsl",
        L"vs_6_0",
        sDirectXCommon->GetDxcUtils(),
        sDirectXCommon->GetDxcCompiler(),
        sDirectXCommon->GetIncludeHandler()
    );
    assert(property.vertexShaderBlob != nullptr);

    property.pixelShaderBlob = CompileShader(
        L"Resources/shader/Object3d.PS.hlsl",
        L"ps_6_0",
        sDirectXCommon->GetDxcUtils(),
        sDirectXCommon->GetDxcCompiler(),
        sDirectXCommon->GetIncludeHandler()
    );
    assert(property.pixelShaderBlob != nullptr);

    // グラフィックスパイプラインステートの設定
    graphicsPipelineStateDesc.pRootSignature = property.rootSignature.Get(); // ルートシグネチャ
    graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; // 入力レイアウト
    graphicsPipelineStateDesc.VS = { property.vertexShaderBlob->GetBufferPointer(), property.vertexShaderBlob->GetBufferSize() }; // 頂点シェーダー
    graphicsPipelineStateDesc.PS = { property.pixelShaderBlob->GetBufferPointer(), property.pixelShaderBlob->GetBufferSize() }; // ピクセルシェーダー
    graphicsPipelineStateDesc.BlendState = blendDesc; // ブレンドステート
    graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; // ラスタライザーステート
    graphicsPipelineStateDesc.NumRenderTargets = 1; // レンダーターゲット数
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // レンダーターゲットのフォーマット
    graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // プリミティブのトポロジタイプ
    graphicsPipelineStateDesc.SampleDesc.Count = 1; // マルチサンプリングの数
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // サンプルマスク
    graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc_; // デプスステンシルステート
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT; // デプスステンシルビューのフォーマット

    // パイプラインステートオブジェクトの生成
    property.graphicsPipelineState = nullptr;
    hr_ = sDirectXCommon->GetDevice()->CreateGraphicsPipelineState(
        &graphicsPipelineStateDesc,
        IID_PPV_ARGS(&property.graphicsPipelineState)
    );
    assert(SUCCEEDED(hr_));
}

/// <summary>
/// シェーダーとリソースの関係を設定する関数
/// </summary>
void PSOAnimationModel::CreateRootSignature() {
    // DirectXCommonのインスタンスを取得
    DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();

    // ルートシグネチャのフラグを設定
    descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    // ルートパラメータの設定
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 定数バッファビュー
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使用
    rootParameters[0].Descriptor.ShaderRegister = 0; // レジスタ番号0

    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // 定数バッファビュー
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // 頂点シェーダーで使用
    rootParameters[1].Descriptor.ShaderRegister = 0; // レジスタ番号0

    // ディスクリプタレンジの設定
    descriptorRange_[0].BaseShaderRegister = 0; // レジスタ番号0
    descriptorRange_[0].NumDescriptors = 1; // ディスクリプタ数
    descriptorRange_[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // シェーダーリソースビュー
    descriptorRange_[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    descriptorRange_[1].BaseShaderRegister = 1; // レジスタ番号1
    descriptorRange_[1].NumDescriptors = 1; // ディスクリプタ数
    descriptorRange_[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // シェーダーリソースビュー
    descriptorRange_[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    // ルートパラメータの設定（続き）
    rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ディスクリプタテーブル
    rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使用
    rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange_[0]; // ディスクリプタレンジへのポインタ
    rootParameters[2].DescriptorTable.NumDescriptorRanges = 1; // ディスクリプタレンジ数

    rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[3].Descriptor.ShaderRegister = 1;

    rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootParameters[4].Descriptor.ShaderRegister = 2;

    rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ディスクリプタテーブル
    rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // 頂点シェーダーで使用
    rootParameters[5].DescriptorTable.pDescriptorRanges = &descriptorRange_[0];
    rootParameters[5].DescriptorTable.NumDescriptorRanges = 1;

    rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ディスクリプタテーブル
    rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使用
    rootParameters[6].DescriptorTable.pDescriptorRanges = &descriptorRange_[1];
    rootParameters[6].DescriptorTable.NumDescriptorRanges = 1;

    // スタティックサンプラーの設定
    staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // 線形フィルタ
    staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 繰り返しモード
    staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較なし
    staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // 最大レベルのミップマップ
    staticSamplers[0].ShaderRegister = 0; // レジスタ番号0
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // ピクセルシェーダーで使用

    // ルートシグネチャの記述子にパラメータを設定
    descriptionRootSignature.pParameters = rootParameters;
    descriptionRootSignature.NumParameters = _countof(rootParameters);
    descriptionRootSignature.pStaticSamplers = staticSamplers;
    descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

    // ルートシグネチャのシリアライズと生成
    property.errorBlob = nullptr;
    hr_ = D3D12SerializeRootSignature(
        &descriptionRootSignature,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &property.signatureBlob,
        &property.errorBlob
    );
    if (FAILED(hr_)) {
        ConsoleLog(reinterpret_cast<char*>(property.errorBlob->GetBufferPointer()));
        assert(false);
    }

    property.rootSignature = nullptr;
    hr_ = sDirectXCommon->GetDevice()->CreateRootSignature(
        0,
        property.signatureBlob->GetBufferPointer(),
        property.signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&property.rootSignature)
    );
    assert(SUCCEEDED(hr_));
}

/// <summary>
/// 頂点シェーダーへ渡す頂点データの指定を行う関数
/// </summary>
void PSOAnimationModel::SetInputLayout() {
    // 頂点入力要素の設定
    inputElementDescs[0].SemanticName = "POSITION";
    inputElementDescs[0].SemanticIndex = 0;
    inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDescs[1].SemanticName = "TEXCOORD";
    inputElementDescs[1].SemanticIndex = 0;
    inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDescs[2].SemanticName = "NORMAL";
    inputElementDescs[2].SemanticIndex = 0;
    inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDescs[3].SemanticName = "WEIGHT";
    inputElementDescs[3].SemanticIndex = 0;
    inputElementDescs[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputElementDescs[3].InputSlot = 1; // スロット1を使用
    inputElementDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDescs[4].SemanticName = "INDEX";
    inputElementDescs[4].SemanticIndex = 0;
    inputElementDescs[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;
    inputElementDescs[4].InputSlot = 1; // スロット1を使用
    inputElementDescs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    // 入力レイアウトの設定
    inputLayoutDesc.pInputElementDescs = inputElementDescs;
    inputLayoutDesc.NumElements = _countof(inputElementDescs);
}

/// <summary>
/// ピクセルシェーダーからの出力をどのように書き込むか設定する関数
/// </summary>
void PSOAnimationModel::SetBlendState() {
    // ブレンドステートの設定
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
}

/// <summary>
/// ラスタライザーに関する設定を行う関数
/// </summary>
void PSOAnimationModel::SetRasterizerState() {
    // ラスタライザーステートの設定
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK; // 背面カリング
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID; // 塗りつぶし
}

/// <summary>
/// デプスバッファを生成する関数
/// </summary>
void PSOAnimationModel::CreateDepth() {
    // デプスステンシルステートの設定
    depthStencilDesc_.DepthEnable = true; // 深度テストを有効化
    depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; // 深度書き込みを許可
    depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; // 深度比較関数
}

/// <summary>
/// シングルトンインスタンスを取得する関数
/// </summary>
/// <returns>PSOAnimationModelのインスタンス</returns>
PSOAnimationModel* PSOAnimationModel::GetInstance() {
    static PSOAnimationModel instance;
    return &instance;
}
