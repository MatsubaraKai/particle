#pragma once
#include <d3d12.h>
#include <string>
#include <string>
#include <format>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>
#include <wrl.h>
#include "function.h"
#include <unordered_map>
#include <DirectXTex.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
// テクスチャ1枚分のデータ
struct TextureData {
	DirectX::TexMetadata metaData;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	uint32_t srvIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

};
/*----------------------------------------------------------
   このクラスはシングルトンパターンを元に設計する
--------------------------------------------------------------*/
class TextureManager
{
public:
	void Init();
private:
	TextureManager() = default;
	~TextureManager() = default;
	const TextureManager& operator=(const TextureManager&) = delete;
public:
	static TextureManager* GetInstance();
	static int PlusIndex();
	// 格納しその保管番号を返す
	static int StoreTexture(const std::string& filePath);
	void Release();

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

	// DirectX12のTextureResourceを作る
	static Microsoft::WRL::ComPtr <ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr <ID3D12Device> device, const DirectX::TexMetadata& matdata);

	static Microsoft::WRL::ComPtr <ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr <ID3D12Resource>, const DirectX::ScratchImage& mipImages);



	uint32_t GetIndex() { return  kParIndez; }
private:
	// デスクリプタ―の数
	static const size_t kNumDescriptors = 256;

	// エラー検知用変数
	HRESULT hr_;

	// 現在空白のHeap位置を表す
	static int kSRVIndexTop;
	static int kParIndez;

	// テクスチャデータ
	static std::unordered_map<std::string, TextureData> textureDatas_;
	static Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[kNumDescriptors];
};

