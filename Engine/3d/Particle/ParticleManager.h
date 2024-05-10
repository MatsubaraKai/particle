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

class ParticleManager
{
public:
	struct particleGroup {
		// 実際に頂点リソースを作る
		Microsoft::WRL::ComPtr <ID3D12Resource> materialResource;
		//std::list<Particle> particles;
		uint32_t kNumMaxInstance;
	};
public:
	// シングルトンパターン
	static ParticleManager* GetInstance();

	ParticleManager() = default;
	~ParticleManager() = default;
	const ParticleManager& operator=(const ParticleManager&) = delete;

public: // base 
	void Initialize();
	void Update();
	void Draw();

public:
	void CreateParticleGroup(const std::string name, const std::string textureFilePath);

private:
	std::unordered_map<std::string, particleGroup> particleGrops_;
};

