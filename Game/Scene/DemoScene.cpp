#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Loder.h"

void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.x = 3.5f;
	cameraPos.y = 1.0f;
	cameraPos.z = -15.0f;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();
	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/white.png");
	demoSprite = new Sprite();
	demoSprite->Init({ 0.0f,0.0f }, { 600.0f,600.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = false;
	worldTransform.Initialize();
	worldTransform.translation_.x = 0;
	worldTransform2.Initialize();
	worldTransform2.translation_.x = 5;
	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/AnimatedCube", "AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/ball", "ball.obj");
	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");

	Loder::LoadJsonFile("Resources", "TL10", object3d_, camera);

	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();

	//object3d->SetModel("sneakWalk.gltf");
	//object3d->SetModel("ball.obj");
	object3d->SetAnimationModel("sneakWalk.gltf");
	object3d2->SetModel("AnimatedCube.gltf");

	
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
	camera->Update();
	camera->CameraDebug();
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
	}

	short leftStickX = joyState.Gamepad.sThumbLX;
	if (leftStickX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		worldTransform.translation_.x -= 0.01f;
		worldTransform.rotation_.y = -rotateSize_;
	}
	if (leftStickX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		worldTransform.translation_.x += 0.01f;
		worldTransform.rotation_.y = rotateSize_;
	}

	sceneTime++;
	////カメラの更新
	demoSprite->Update();

	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		rotateSize_ = 0.0f;
	}
	if (Input::GetInstance()->TriggerKey(DIK_D)) {
		rotateSize_ = 0.05f;
	}
	for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
		(*itr)->Update();
	}
	object3d_[0]->ModelDebug("model");
	object3d_[1]->ModelDebug("model2");
	object3d_[2]->ModelDebug("model3");
	object3d->SetWorldTransform(worldTransform);
	object3d2->SetWorldTransform(worldTransform2);

	object3d->ModelDebug("modelaa");
	object3d->Update();
	object3d2->Update();
}
void DemoScene::Draw()
{
	for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
		(*itr)->Draw(textureHandle, camera);
	}
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	object3d->Draw(textureHandle,camera);
	object3d2->Draw(textureHandle2, camera);
	particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, demoRandPro, false);
}

void DemoScene::PostDraw()
{
	postProcess_->Draw();
}

void DemoScene::Release() {
}

// ゲームを終了
int DemoScene::GameClose()
{
	return false;
}
