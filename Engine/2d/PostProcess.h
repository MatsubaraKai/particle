#pragma once

#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include <cassert>

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
#include "WorldTransform.h"
#include "TextureManager.h"
#include "Camera.h"
#include "IPostEffectState.h"
#include "Effect/Bloom.h"
#include "Effect/DepthOutline.h"
#include "Effect/FullScreen.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

class PostProcess
{
public:



	PostProcess();
	~PostProcess();

	void Init();
	void Update();
	void Draw();
	void Release();
	void SetTextureManager(TextureManager* textureManager) {
		textureManager_ = textureManager;
	}
	D3D12_VERTEX_BUFFER_VIEW CreateBufferView();

	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetThreshold(float threshold) { threshold_ = threshold; }
	void SetDeviation(float deviation) { deviation_ = deviation; }

	float GetThreshold() { return threshold_; }
	float GetDeviation() { return deviation_; }
	BloomInfo GetBloominfo() { return bloomInfo_; }
	void SetBloomInfo(BloomInfo bloominfo) { bloomInfo_ = bloominfo; }

	FullScreenInfo GetHSVInfo() { return hsvInfo_; }
	void SetHSVInfo(FullScreenInfo hsvInfo) { hsvInfo_ = hsvInfo; }

	Vector2 GetRandom() { return random_; }
	void Setrandom(Vector2 rand) { random_ = rand; }

	Vector2 GetDarkness() { return darkness_; }
	void SetDarkness(Vector2 darkness) { darkness_ = darkness; }
	Vector3 GetColorValue() { return valueColor; }
	void SetvalueColor(Vector3 valueColor_) { valueColor = valueColor_; }
	Camera* GetCamera() { return camera_; }
	uint32_t GetNoisetex() { return noiseTexture_; }

	float GetFarClip() { return depthOutlineInfo_.farClip; }
	void SerFarClip(const float& farClip) { depthOutlineInfo_.farClip = farClip; }

private:
	// シーンを保持するメンバ変数
	//std::unique_ptr<IPostEffectState> effectArr_[POSTEFFECTMODE::EFFECTNUM];

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_ = 0;
	int prevSceneNo_ = 0;

	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResourceSprite_ = nullptr;
	WinAPI* sWinAPI;
	TextureManager* textureManager_ = nullptr;
	// 頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};

	VertexData* vertexDataSprite_ = nullptr;

	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr < ID3D12Resource> transformationMatrixResouceSprite;
	// データを書き込む

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

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

	//ParticlePro particles_[kNumMaxInstance];
	std::list<Transform>  transforms_;
	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResource;

	// 平行光源用
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource;
	// データを書き込む
	DirectionalLight* directionalLightData;
	Transform transformUv;

	uint32_t SRVIndex_;

	uint32_t noiseTexture_;
	//
	Camera* camera_ = nullptr;

	// 
	float threshold_ = 0.0f;
	float deviation_ = 2.0f;
	Vector2 darkness_{ 16.0f,0.8f };
	Vector2 random_{ 16.0f,0.8f };
	Vector3 valueColor = { 0.2125f, 0.7154f, 0.0721f };

	BloomInfo bloomInfo_ = { 10.0f,0.3f,1.0f, 1 };
	DepthOutlineInfo depthOutlineInfo_;
	FullScreenInfo hsvInfo_;
};

