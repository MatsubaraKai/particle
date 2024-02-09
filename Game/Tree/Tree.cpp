#include "Tree.h"

Tree::Tree() {

}

Tree::~Tree() {

}

void Tree::Init() {
	model_ = new Model();
	model_->Initialize("Resources/tree", "tree.obj", color);
	texture_ = TextureManager::StoreTexture("Resources/tree/green.png");
	worldTransform_.Initialize();
}

void Tree::Update() {

	worldTransform_.UpdateMatrix();
}

void Tree::Draw(Camera* camera) {
	model_->Draw(worldTransform_, texture_, camera, color);
}

void Tree::Release()
{

}
