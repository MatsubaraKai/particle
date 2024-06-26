﻿#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.z = -10;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();
	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/white.png");
	textureHandle3 = TextureManager::StoreTexture("Resources/AnimatedCube/AnimatedCube_BaseColor.png");
	demoSprite = new Sprite();
	demoSprite->Init({ 0.0f,0.0f }, { 600.0f,600.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = true;
	worldTransform.Initialize();
	worldTransform.translation_.x = 0;
	worldTransform2.Initialize();
	worldTransform2.translation_.x = 5;
	worldTransform3.Initialize();
	worldTransform3.translation_.x = 2;
	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	worldTransform3.UpdateMatrix();

	ModelManager::GetInstance()->LoadModel("Resources/plane", "plane.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	ModelManager::GetInstance()->LoadModel("Resources/AnimatedCube", "AnimatedCube.gltf");
	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();
	object3d3 = new Object3d();
	object3d3->Init();

	object3d->SetModel("plane.gltf");
	object3d2->SetModel("ball.obj");
	object3d3->SetModel("AnimatedCube.gltf");
	particle = new Particle();
	particle2 = new Particle();

	demoRandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{0.0f,2.0f}
	};

	demoEmitter_.count = 6;
	demoEmitter_.frequency = 0.02f;
	demoEmitter_.frequencyTime = 0.0f;
	demoEmitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(demoEmitter_);
	particle2->Initialize(demoEmitter_);
}

void DemoScene::Update()
{
	if (input->PushKey(DIK_W)) {
		worldTransform.translation_.y += 0.1f;
	}
	if (input->PushKey(DIK_A)) {
		worldTransform.translation_.x -= 0.1f;
	}
	if (input->PushKey(DIK_S)) {
		worldTransform.translation_.y -= 0.1f;
	}
	if (input->PushKey(DIK_D)) {
		worldTransform.translation_.x += 0.1f;
	}
	sceneTime++;
	////カメラの更新
	camera->Update();
	demoSprite->Update();
	worldTransform.rotation_.y += 0.05f;
	worldTransform3.rotation_.y += 0.05f;
	object3d->SetWorldTransform(worldTransform);
	object3d2->SetWorldTransform(worldTransform2);
	object3d3->SetWorldTransform(worldTransform3);

	object3d->Update();
	object3d2->Update();
	object3d3->Update();

}
void DemoScene::Draw()
{
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	object3d->Draw(textureHandle, camera);
	object3d2->Draw(textureHandle2, camera);
	object3d3->Draw(textureHandle3, camera);
	particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, demoRandPro, false);
}

void DemoScene::Release() {
}

// ゲームを終了
int DemoScene::GameClose()
{
	return false;
}