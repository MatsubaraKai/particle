#pragma once
#include "Collider.h"
#include<list>
class GameScene;
class CollisionManager {
public:
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	void CheckAllCollision();
	// ClliderにPushする
	void PushClider(Collider* collider);
	void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }

private:
	GameScene* gameScene_ = nullptr;
	std::list<Collider*> colliders_;
};