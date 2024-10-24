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
#include "Bloom.h"
#include "DepthOutline.h"
#include "FullScreen.h"
#include "GrayScale.h"
#include "Vignette.h"
#include "Dissolve.h"
#include "GaussianBlur.h"
#include "RadialBlur.h"
#include "Random.h"
#include "Fog.h"
#include "HSVFilter.h"
#include "LuminanceBasedOutline.h"
#include "OutlineBlack.h"
#include "OutlineBlue.h"
#include "TVEffect.h"
#include "OutlinePurple.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxcompiler.lib")

/**
* @file PostProcess.h
* @brief PostProcess
*/
class PostProcess
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostProcess();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~PostProcess();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Init();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw();

    /// <summary>
    /// リソースの解放
    /// </summary>
    void Release();

    /// <summary>
    /// テクスチャマネージャーの設定
    /// </summary>
    /// <param name="textureManager">テクスチャマネージャーのポインタ</param>
    void SetTextureManager(TextureManager* textureManager) {
        textureManager_ = textureManager;
    }

    /// <summary>
    /// 頂点バッファビューを作成する
    /// </summary>
    /// <returns>作成された頂点バッファビュー</returns>
    D3D12_VERTEX_BUFFER_VIEW CreateBufferView();

    /// <summary>
    /// カメラを設定する
    /// </summary>
    /// <param name="camera">カメラのポインタ</param>
    void SetCamera(Camera* camera) { camera_ = camera; }

    /// <summary>
    /// スレッショルド値を設定する
    /// </summary>
    /// <param name="threshold">スレッショルド値</param>
    void SetThreshold(float threshold) { threshold_ = threshold; }

    /// <summary>
    /// 偏差値を設定する
    /// </summary>
    /// <param name="deviation">偏差値</param>
    void SetDeviation(float deviation) { deviation_ = deviation; }

    /// <summary>
    /// スレッショルド値を取得する
    /// </summary>
    /// <returns>スレッショルド値</returns>
    float GetThreshold() { return threshold_; }

    /// <summary>
    /// 偏差値を取得する
    /// </summary>
    /// <returns>偏差値</returns>
    float GetDeviation() { return deviation_; }

    /// <summary>
    /// Bloom情報を取得する
    /// </summary>
    /// <returns>Bloom情報</returns>
    BloomInfo GetBloominfo() { return bloomInfo_; }

    /// <summary>
    /// Bloom情報を設定する
    /// </summary>
    /// <param name="bloominfo">Bloom情報</param>
    void SetBloomInfo(BloomInfo bloominfo) { bloomInfo_ = bloominfo; }

    /// <summary>
    /// HSVフィルター情報を取得する
    /// </summary>
    /// <returns>HSVフィルター情報</returns>
    HSVFilterInfo GetHSVFilterinfo() { return hsvFilterInfo_; }

    /// <summary>
    /// HSVフィルター情報を設定する
    /// </summary>
    /// <param name="hsvfilterinfo">HSVフィルター情報</param>
    void setHSVFilterInfo(HSVFilterInfo hsvfilterinfo) { hsvFilterInfo_ = hsvfilterinfo; }

    /// <summary>
    /// フルスクリーンのHSV情報を取得する
    /// </summary>
    /// <returns>HSV情報</returns>
    FullScreenInfo GetHSVInfo() { return hsvInfo_; }

    /// <summary>
    /// フルスクリーンのHSV情報を設定する
    /// </summary>
    /// <param name="hsvInfo">HSV情報</param>
    void SetHSVInfo(FullScreenInfo hsvInfo) { hsvInfo_ = hsvInfo; }

    /// <summary>
    /// ランダムな値を取得する
    /// </summary>
    /// <returns>ランダム値</returns>
    Vector2 GetRandom() { return random_; }

    /// <summary>
    /// ランダム値を設定する
    /// </summary>
    /// <param name="rand">ランダム値</param>
    void Setrandom(Vector2 rand) { random_ = rand; }

    /// <summary>
    /// 暗さの値を取得する
    /// </summary>
    /// <returns>暗さの値</returns>
    Vector2 GetDarkness() { return darkness_; }

    /// <summary>
    /// 暗さの値を設定する
    /// </summary>
    /// <param name="darkness">暗さの値</param>
    void SetDarkness(Vector2 darkness) { darkness_ = darkness; }

    /// <summary>
    /// 色の値を取得する
    /// </summary>
    /// <returns>色の値</returns>
    Vector3 GetColorValue() { return valueColor; }

    /// <summary>
    /// 色の値を設定する
    /// </summary>
    /// <param name="valueColor_">設定する色の値</param>
    void SetvalueColor(Vector3 valueColor_) { valueColor = valueColor_; }

    /// <summary>
    /// カメラを取得する
    /// </summary>
    /// <returns>カメラのポインタ</returns>
    Camera* GetCamera() { return camera_; }

    /// <summary>
    /// ノイズテクスチャを取得する
    /// </summary>
    /// <returns>ノイズテクスチャのID</returns>
    uint32_t GetNoisetex() { return noiseTexture_; }

    /// <summary>
    /// ファークリップを取得する
    /// </summary>
    /// <returns>ファークリップの値</returns>
    float GetFarClip() { return depthOutlineInfo_.farClip; }

    /// <summary>
    /// ファークリップを設定する
    /// </summary>
    /// <param name="farClip">ファークリップの値</param>
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

	HSVFilterInfo hsvFilterInfo_ = { 0.3f,0.5f,0.6f };
	BloomInfo bloomInfo_ = { 10.0f,0.3f,1.0f, 1 };
	DepthOutlineInfo depthOutlineInfo_;
	FullScreenInfo hsvInfo_;
};

