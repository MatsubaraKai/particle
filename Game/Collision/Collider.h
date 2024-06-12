#pragma once
#include"Vector3.h"
#include<cstdint>
class Collider {
public:


	// Getter
	// 半径を取得
	int GetRadius() { return radius_; }

	// Setter
	void SetRadius(int radius) { radius_ = radius; }

	virtual void OnCollision() = 0;

	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() const = 0;

	// 衝突属性（自分）を取得
	uint32_t GetCollisonAttribute() { return collisionAttribute_; }
	// 衝突属性（自分）を設定
	void SetCollisonAttribute(uint32_t collisionAttribute);
	// 衝突マスク（相手）を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }
	// 衝突マスク（相手）を設定
	void SetCollisionMask(uint32_t CollisionMask);

private:
	// 衝突半径
	int radius_ = 2;

	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xFFFFFFFF;
	// 衝突マスク（相手）
	uint32_t CollisionMask_ = 0xffffffff;

};
