#pragma once
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "PSO.h"
#include "Mesh.h"
#include "TextureManager.h"

#include "Transform.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "vertexData.h"
#include "ModelData.h"
#include "MaterialData.h"
#include "mathFunction.h"
#include "Material.h"
#include "TransformationMatrix.h"

#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
class DirectXCommon;
class WinAPI;
class TextureManager;
class Camera;
class PSO;
class Model
{
public:
	ModelData modelData_;
	TransformationMatrix* wvpData;
	ModelData GetModelData() { return modelData_; }
	Model();
	~Model();
	void Initialize(const std::string& directoryPath, const std::string& filename, const Vector4& color);
	void Update();
	void Draw(WorldTransform worldTransform, uint32_t texture, Camera* camera,const Vector4 &color);


	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath,const std::string& filename);
private:
	HRESULT hr;
	// RootSignature作成
	
	DirectXCommon* directXCommon_;
	WinAPI* sWinAPI_;
	TextureManager* textureManager_ = nullptr;
	PSO* pso_ = nullptr;
	
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

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
	
	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;
	// データを書き込む
	DirectionalLight* directionalLightData;
	Transform transformUv;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	Camera* camera_ = nullptr;
};

