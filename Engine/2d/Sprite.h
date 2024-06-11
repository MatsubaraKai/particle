#pragma once
#include "PSOSprite.h"

#include <DirectXTex.h>
#include <string>
#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>

#include "function.h"
#include <wrl.h>
#include "Transform.h"
#include "VertexData.h"
#include "Vector4.h"
#include"Vector3.h"
#include"Vector2.h"
#include "Matrix4x4.h"
#include "Material.h"
#include "DirectionLight.h"
#include "TransformationMatrix.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
class WinAPI;
class DirectXCommon;
class Camera;
class Mesh;
class TextureManager;
class Sprite
{
public:
	Sprite();
	~Sprite();

	void Init(const Vector2& pos, const Vector2& size, const Vector2& anchorPoint, const Vector4& color, const std::string& filePath);
	void Update();
	void Draw(uint32_t texture, const Vector4& color);
	void Release();
	Transform GetTransform() {
		return transform_;
	}
	// getter
	const Vector2& GetPosition() const { return position_; }
	const Vector2& GetSize() const { return size_; }
	const Vector2& GetAnchorPoint()const { return anchorPoint_; }
	const Vector2& GetTextureLeftTop()const { return textureleftTop_; }
	const Vector2& GetTextureSize()const { return textureSize_; }

	// setter
	void SetPosition(const Vector2& position) { this->position_ = position; }
	void SetSize(const Vector2& size) { this->size_ = size; }
	void SetAnchorPoint(const Vector2& anchorPoint) {
		this->anchorPoint_ = anchorPoint;
	}
	void SetTextureLeftTop(const Vector2& textureleftTop) {
		this->textureleftTop_ = textureleftTop;
	}
	void SetTextureSize(const Vector2& textureSize) {
		this->textureSize_ = textureSize;
	}
	D3D12_VERTEX_BUFFER_VIEW CreateBufferView();
private:
	PSOSprite* pso_ = nullptr;
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResourceSprite_ = nullptr;
	WinAPI* sWinAPI;
	DirectXCommon* sDirectXCommon;
	Mesh* mesh_;
	Vector2 position_ = { 0.0f, 0.0f };
	Vector2 size_ = { 1.0f,1.0f };
	Vector2 anchorPoint_ = { 0.0f,0.0f };
	// テクスチャ左上座標
	Vector2 textureleftTop_ = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 64.0f,64.0f };
	Transform transform_;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};

	VertexData* vertexDataSprite_ = nullptr;

	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResouceSprite;
	// データを書き込む
	TransformationMatrix* transformationMatrixDataSprite = nullptr;

	Transform transformSprite_;

	//D3D12_DESCRIPTOR_RANGE descriptorRange_[1] = {};

	// RootParmeter作成。複数でっていできるので配列。今回は結果１つだけなので長さ1の配列
	//D3D12_ROOT_PARAMETER rootParamerters[1] = {};

	uint32_t* indexDataSprite;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResourceSprite;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};


	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};
	// 頂点リソースにデータを書き込む
	Material* materialData;


	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	// データを書き込む
	DirectionalLight* directionalLightData;
	Transform transformUv;
	Vector2 originSize_;
};

