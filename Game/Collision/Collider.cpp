#include "Collider.h"
#include <cmath>
// 衝突属性（自分）を設定
void Collider::SetCollisonAttribute(uint32_t collisionAttribute) {
	collisionAttribute_ = collisionAttribute;
};
// 衝突マスク（相手）を設定
void Collider::SetCollisionMask(uint32_t CollisionMask) {
	CollisionMask_ = CollisionMask;
};

bool Collider::CheckCollision(const Vector3& playerPos, const Vector3& objectPos, float offsetX, float offsetY, float offsetZ, float collisionDistance) {
    float dx = (objectPos.x + offsetX) - playerPos.x;
    float dy = (objectPos.y + offsetY) - playerPos.y;
    float dz = (objectPos.z + offsetZ) - playerPos.z;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    return distance < collisionDistance;
}