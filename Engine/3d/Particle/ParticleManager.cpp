#include "ParticleManager.h"

ParticleManager* ParticleManager::GetInstance() {
	static ParticleManager instance;
	return &instance;
}

void ParticleManager::Initialize()
{
}

void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
{
}

