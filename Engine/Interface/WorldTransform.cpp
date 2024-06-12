#include "WorldTransform.h"
#include "mathFunction.h"

void WorldTransform::Initialize() {
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}
}

void WorldTransform::CreateConstBuffer() {

}

void WorldTransform::Map() {

}

void WorldTransform::TransferMatrix() {

}

void WorldTransform::UpdateMatrix() {
	// スケール、回転、平行移動を合成して行列を計算する
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列掛ける

	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}


	// 定数バッファに転送する
	//TransferMatrix();

}
