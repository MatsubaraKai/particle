#pragma once
#include <wrl.h>
#include <vector>

#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <d3d12.h>
#include <Transform.h>
#include <Camera.h>
#include <Material.h>
#include "DirectionLight.h"
class Skybox
{
public:
	void Init(const Material& material);
	void Draw(uint32_t texture, const Material& material, const DirectionalLight& dire);
private:
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	VertexData* vertexData_;

	/*色用*/
	//頂点リソースの設定
	// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};




	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;

	// IndexBuffer用リソース
	uint32_t* indexDataSprite;
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW	indexBufferView_{};
	// データを書き込む
	DirectionalLight* directionalLightData;



	//ModelData modelData_;
	ModelData modelData_;

	// 頂点リソースにデータを書き込む
	Material* materialData_;
	Transform transformUv;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	Camera* camera_ = nullptr;

	Matrix4x4 aniMatrix_;
	Matrix4x4 skeMatrix_;
	float animationTime = 0.0f;
};

