#pragma once
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "PSOModel.h"
#include "Mesh.h"
#include "TextureManager.h"
//#include "ModelManager.h"

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

#include "Animation.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")



class Model
{
public:

	ModelData GetModelData() { return modelData_; }
	Model();
	~Model();
	void Initialize(const std::string& directoryPath, const std::string& filePath, const Material& material);
	void Update();
	void Draw(uint32_t texture, const Material& material, const DirectionalLight& dire, uint32_t mapTexture);


	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filePath);
	ModelData LoadGLTFFile(const std::string& directoryPath, const std::string& filePath);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	Matrix4x4 GetAniMatrix() { return aniMatrix_; }
	Matrix4x4 GetSkeMatrix() { return skeMatrix_; }
	// アニメーション読み込み
	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filePath);

	Node ReadNode(aiNode* node);
	void ApplyAnimation(SkeletonData& skeleton, const AnimationData& animation, float animationTime);

private:



	HRESULT hr;
	// RootSignature作成
	//ModelManager* modelManager = nullptr;

	// 頂点リソースにデータを書き込む
	Material* materialData;

	VertexData* vertexData_;
	ModelData modelData_;
	AnimationData animation_;
	SkeletonData skeleton_;
	SkinCluster skinCluster_;


	DirectXCommon* directXCommon_;
	WinAPI* sWinAPI_;
	TextureManager* textureManager_ = nullptr;
	PSO* pso_ = nullptr;

	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	D3D12_VERTEX_BUFFER_VIEW vbvs[2]{};

	/*色用*/
//頂点リソースの設定
// 実際に頂点リソースを作る
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW materialBufferView{};




	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;

	// IndexBuffer用リソース

	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW	indexBufferView_{};
	// データを書き込む
	DirectionalLight* directionalLightData;



	Transform transformUv;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	Camera* camera_ = nullptr;

	Matrix4x4 aniMatrix_;
	Matrix4x4 skeMatrix_;
	float animationTime = 0.0f;
};

