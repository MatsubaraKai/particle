#pragma once
#include "PSOModel.h"
#include "ModelData.h"
#include "MaterialData.h"
#include "vertexData.h"
#include "PSOSkybox.h"
#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <Material.h>
#include <Transform.h>
#include <Camera.h>
#include <WorldTransform.h>
#include "Model.h"
#include "AnimationModel.h"
#include "Object3dCommon.h"
#include <PSOAnimationModel.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")
class Object3d
{
public:
	void Init();
	void Update();
	void Draw(uint32_t texture, Camera* camera);
	void Release();
	void ModelDebug(const char* name);
	void SetModel(Model* model) { model_ = model; }
	void SetModel(const std::string& filePath);
	void SetAnimationModel(const std::string& filePath);
	void SetWorldTransform(const WorldTransform& worldtransform) { worldTransform_ = worldtransform; };
	void SetTransform(Transform transform);
	void SetMapTexture(uint32_t maptexture) { mapTexture_ = maptexture; };
public: // Getter
	WorldTransform GetWorldTransform() { return worldTransform_; }
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	// データを書き込む
	WorldTransform worldTransform_;
private:
	Object3dCommon* objectCommon_ = nullptr;

	HRESULT hr;
	// RootSignature作成
	Model* model_ = nullptr;
	AnimationModel* animationModel_ = nullptr;

	/*移動用*/
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	TransformationMatrix* wvpData;
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW wvpBufferView{};
	Transform transformUv;

	/*カメラ用*/
	Microsoft::WRL::ComPtr < ID3D12Resource> cameraForGPUResource_;
	CameraForGPU* cameraForGPUData_;

	// 環境マップ用Texture
	uint32_t mapTexture_;
};

