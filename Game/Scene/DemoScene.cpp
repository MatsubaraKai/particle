#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.z = -50;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();
	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/AnimatedCube/AnimatedCube_BaseColor.png");
	textureHandle3 = TextureManager::StoreTexture("Resources/white.png");
	demoSprite = new Sprite();
	demoSprite->Init({ 0.0f,0.0f }, { 600.0f,600.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = true;
	worldTransform.Initialize();
	worldTransform.translation_.x = 0;
	worldTransform2.Initialize();
	worldTransform2.translation_.x = 5;
	worldTransform3.Initialize();
	worldTransform3.translation_.x = -2;
	worldTransform3.translation_.y = 5;
	worldTransform4.Initialize();
	worldTransform4.translation_.x = 0;
	worldTransform4.translation_.y = 5;
	worldTransform5.Initialize();
	worldTransform5.translation_.x = 2;
	worldTransform5.translation_.y = 5;

	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	worldTransform3.UpdateMatrix();
	worldTransform4.UpdateMatrix();
	worldTransform5.UpdateMatrix();

	ModelManager::GetInstance()->LoadModel("Resources/AnimatedCube", "AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/human", "walk.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/simpleSkin", "simpleSkin.gltf");
	//ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();
	human = new Object3d();
	human->Init();
	human2 = new Object3d();
	human2->Init();
	simple = new Object3d();
	simple->Init();
	

	object3d->SetModel("AnimatedCube.gltf");
	object3d2->SetModel("AnimatedCube.gltf");
	human->SetModel("sneakWalk.gltf");
	human2->SetModel("walk.gltf");
	simple->SetModel("simpleSkin.gltf");
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
	sceneTime++;
	////カメラの更新
	camera->Update();
	demoSprite->Update();
	object3d->SetWorldTransform(worldTransform);
	object3d2->SetWorldTransform(worldTransform2);
	human->SetWorldTransform(worldTransform3);
	human2->SetWorldTransform(worldTransform4);
	simple->SetWorldTransform(worldTransform5);

	object3d->Update();
	object3d2->Update();
	human->Update();
	human2->Update();
	simple->Update();
}
void DemoScene::Draw()
{
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	object3d->Draw(textureHandle, camera);
	object3d2->Draw(textureHandle2, camera);
	/*human->Draw(textureHandle3, camera);
	human2->Draw(textureHandle3, camera);
	simple->Draw(textureHandle, camera);*/
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