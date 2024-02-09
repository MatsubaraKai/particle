#pragma once
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include <dxcapi.h>

//#include"DirectXCommon.h"
#include "VertexData.h"
#include "Vector4.h"
#include"Vector3.h"
#include"Vector2.h"
#include"Matrix4x4.h"
#include "Transform.h"
#include "Material.h"
#include "TransformationMatrix.h"
#include "DirectionLight.h"
#include "Mesh.h"
#include "PSO.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

class DirectXCommon;
class WinAPI;
class TextureManager;
class Camera;
class Sphere
{
public :
	Sphere();
	~Sphere();
	void Initialize(Camera* camera);
	void Update();
	void Draw(Transform transform, uint32_t texture);
	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}
	void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportmatrix, int color);
	// データを書き込む
	DirectionalLight* directionalLightData;
	Transform transformUv;
	D3D12_VERTEX_BUFFER_VIEW  CreateBufferView();
private:
	PSO* pso_ = nullptr;
	WinAPI* sWinAPI = nullptr;
	DirectXCommon* sDirectXCommon_ = nullptr;
	
	TextureManager* textureManager_ = nullptr;

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

	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	

	/*移動用*/
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource;
	// データを書き込む
	TransformationMatrix* wvpData;

	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	D3D12_DESCRIPTOR_RANGE descriptorRange_[1] = {};
	//ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};




	Camera* camera_ = nullptr;



	


};

