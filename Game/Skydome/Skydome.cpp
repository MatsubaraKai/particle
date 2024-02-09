#include "Skydome.h"

Skydome::Skydome() {

}

Skydome::~Skydome() {

}

void Skydome::Init() {
	model_ = new Model();
	model_->Initialize("Resources/skydome", "skydome.obj", color);
	texture_ = TextureManager::StoreTexture("Resources/skydome/sky.png");
	worldTransform_.Initialize();
}

void Skydome::Update() {

	worldTransform_.rotation_.y += 0.005f;
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(Camera* camera) {
	model_->Draw(worldTransform_, texture_, camera, color);
}

void Skydome::Release()
{

}
