#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>

#include "WinAPI.h"
#include "DirectXCommon.h"
#include "Mesh.h"
#include "PSO.h"
#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Material.h"
#include "DirectionLight.h"
#include "TransformationMatrix.h"
#include "Transform.h"
#include "WorldTransform.h"
#include "Camera.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")


class TextureManager;

class Triangle
{

public:

	void Initialize(Camera* camera, Vector4 DrawColor);
	void Update(Camera camera, Vector4 DrawColor);
	void Draw(WorldTransform worlsTransform, Camera* camera, uint32_t texture, Vector4 DrawColor);
	void Release();
	D3D12_VERTEX_BUFFER_VIEW  CreateBufferView();
	D3D12_RESOURCE_DESC  CreateBufferResourceDesc(size_t sizeInBytes);
	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}


	HRESULT hr;
	/*頂点用*/
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// 頂点リソースにデータを書き込む
	VertexData* vertexData_;


	/*色用*/
	//頂点リソースの設定
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む
	Material* materialData;

	/*移動用*/
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource;
	// データを書き込む
	TransformationMatrix* TransformationData;

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};

	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	// データを書き込む
	DirectionalLight* directionalLightData;

	DirectXCommon* sDirectXCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;
	Camera *camera_ = nullptr;

	//ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
};

