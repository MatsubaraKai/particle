#pragma once
#include <DirectXTex.h>
#include <string>
#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>
#include <wrl.h>
#include "function.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
/*----------------------------------------------------------
   このクラスはシングルトンパターンのを元に設計する
--------------------------------------------------------------*/
class DirectXCommon;
class WinAPI;
class TextureManager
{
public:
	static TextureManager* GetInstance();
	TextureManager() = default;
	~TextureManager() = default;
	const TextureManager& operator=(const TextureManager&) = delete;
	static int PlusIndex();
	// デスクリプタ―の数
	static const size_t kNumDescriptors = 256;

	/// <summary>
	/// テクスチャ
	/// </summary>
	//struct Texture {
	//	// テクスチャリソース
	//	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	//	// シェーダリソースビューのハンドル(CPU)
	//	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//	// シェーダリソースビューのハンドル(GPU)
	//	D3D12_CPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//	// 名簿
	//	std::string name;
	//};
	// 
	// 格納しその保管番号を返す
	static int StoreTexture(const std::string& filePath);
	void Release();
	void SetTexture();
	void SetDirectXCommon();

	static DirectX::ScratchImage LoadTexture(const std::string& filePath);
	
	// エラー検知用変数
	HRESULT hr_;

	static int index_ ;
	
	// DirectX12のTextureResourceを作る
	static Microsoft::WRL::ComPtr <ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr <ID3D12Device> device, const DirectX::TexMetadata& matdata);

	
	//D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	//SRV
	const static uint32_t SRVSizes = 128;
	// metaDataを基にSRVの設定
	
	// Resourceの生成
	static Microsoft::WRL::ComPtr <ID3D12Resource> textureResource_[SRVSizes];

	static D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[SRVSizes];
	static D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[SRVSizes];
	
	static D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_[SRVSizes];
	//D3D12_DESCRIPTOR_RANGE descriptorRange_[1] = {};

	static void UploadTextureData(Microsoft::WRL::ComPtr <ID3D12Resource>, const DirectX::ScratchImage& mipImages);
};

