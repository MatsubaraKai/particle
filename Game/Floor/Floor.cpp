#include "Floor.h"
#include "ImGuiCommon.h"
Floor::Floor() {

}
Floor::~Floor() {

}

void Floor::Init() {
	input = Input::GetInstance();
	worldTransform_.Initialize();
	texture_ = TextureManager::StoreTexture("Resources/demo_floor/floorimage.png");
	model_ = new Model();
	model_->Initialize("Resources/demo_floor", "FLOOR.obj", color);
}

void Floor::Update() {
	worldTransform_.UpdateMatrix();
	ImGui::Begin("Demo_Floor");
	ImGui::DragFloat4("color", &color.x, 0.01f);
	ImGui::End();
}

void Floor::Draw(Camera* camera) {
	model_->Draw(worldTransform_, texture_, camera, color);
}

void Floor::Release()
{
}

