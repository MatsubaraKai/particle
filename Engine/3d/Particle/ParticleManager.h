#pragma once
#include <DirectXTex.h>
#include <string>
#include <string>
#include <format>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>
#include <random>
#include <wrl.h>
#include <list>
#include "Particle.h"

/**
* @file ParticleManager.h
* @brief パーティクルの管理を行うクラス
*/
class ParticleManager
{
public:
    struct particleGroup {
        // 実際に頂点リソースを作る
        Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
        //std::list<Particle> particles;
        uint32_t kNumMaxInstance;
    };

public:
    /// <summary>
    /// シングルトンのインスタンスを取得する
    /// </summary>
    /// <returns>ParticleManagerのインスタンス</returns>
    static ParticleManager* GetInstance();

    /// <summary>
    /// デフォルトコンストラクタ
    /// </summary>
    ParticleManager() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~ParticleManager() = default;

    /// <summary>
    /// 代入演算子を削除（コピー不可）
    /// </summary>
    /// <param name="other">コピー元のオブジェクト</param>
    /// <returns>代入不可</returns>
    const ParticleManager& operator=(const ParticleManager&) = delete;

public: // base 
    /// <summary>
    /// 初期化処理を行う
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新処理を行う
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理を行う
    /// </summary>
    void Draw();

public:
    /// <summary>
    /// パーティクルグループを作成する
    /// </summary>
    /// <param name="name">パーティクルグループの名前</param>
    /// <param name="textureFilePath">テクスチャファイルのパス</param>
    void CreateParticleGroup(const std::string name, const std::string textureFilePath);

private:
    std::unordered_map<std::string, particleGroup> particleGrops_;
};
