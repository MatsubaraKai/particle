#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"

void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.z = -15;
	cameraPos.x = 3.5f;
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
	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();

	object3d->SetModel("sneakWalk.gltf");
	object3d2->SetModel("sneakWalk.gltf");
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
	camera->CameraDebug();
	demoSprite->Update();
	/*if (Input::GetInstance()->TriggerKey(DIK_A)) {
		rotateSize_ = 0.0f;
	}
	if (Input::GetInstance()->TriggerKey(DIK_D)) {
		rotateSize_ = 0.05f;
	}*/
	

	/*worldTransform.translation_.x += rotateSize_;
	worldTransform.rotation_.y += rotateSize_;
	worldTransform2.rotation_.z += rotateSize_;*/
	object3d->SetWorldTransform(worldTransform);
	object3d2->SetWorldTransform(worldTransform2);
	Move();
	object3d->Update();
	object3d2->Update();
	object3d->ModelDebug("1");
	object3d2->ModelDebug("2");

}
void DemoScene::Draw()
{
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	object3d->Draw(textureHandle, camera);
	object3d2->Draw(textureHandle2, camera);
	/*particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, demoRandPro, false);*/
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

void DemoScene::Move()
{
	//ゲームパットの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	Vector3 move = { 0.0f,0.0f,0.0f };
	//移動
	if (Input::GetInstance()->TriggerKey(DIK_W))
	{
		move.z = PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	if (Input::GetInstance()->TriggerKey(DIK_S))
	{
		move.z = -PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	if (Input::GetInstance()->TriggerKey(DIK_A))
	{
		move.x = -PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	if (Input::GetInstance()->TriggerKey(DIK_D))
	{
		move.x = PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	// Y軸周り角度(θy)	歩いている方向に顔を向ける
	worldTransform.rotation_.y = LerpShortAngle(worldTransform.rotation_.y, angle_, 0.1f);
	worldTransform.translation_.x += move.x;
	worldTransform.translation_.z += move.z;
	if (Input::GetInstance()->GetJoystickState(0,joyState))
	{
		const float threshold = 0.9f;
		bool isMoving = false;
		move = { 0,0,0 };

		if (joyState.Gamepad.sThumbLX != 0 || joyState.Gamepad.sThumbLY != 0)
		{
			// 移動量
			move =
			{
				(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
				0.0f,
				(float)joyState.Gamepad.sThumbLY / SHRT_MAX
			};

			float inputMagnitude = Length(move);

			// スティックの入力が一定の閾値以上の場合のみ移動処理を実行
			if (inputMagnitude > threshold)
			{
				isMoving = true;

				// スティックの入力に応じて速度を調整する
				float adjustedSpeed = PlayerSpeed * inputMagnitude;

				// 最大速度を超えないようにする
				if (adjustedSpeed > 0.05f)
				{
					adjustedSpeed = 0.05f;
				}

				// 実際の移動量を計算
				move.x *= adjustedSpeed;
				move.z *= adjustedSpeed;
				// 目標角度の算出
				angle_ = std::atan2(move.x, move.z);
				// Y軸周り角度(θy)	歩いている方向に顔を向ける
				worldTransform.rotation_.y = LerpShortAngle(worldTransform.rotation_.y, angle_, 0.1f);
				worldTransform.translation_.x += move.x;
				worldTransform.translation_.z += move.z;
			}
		}
	}
}

float DemoScene::Lerp(const float& a, const float& b, float t) {
	float result{};

	result = a + b * t;

	return result;
}

// 最短角度補間
float DemoScene::LerpShortAngle(float a, float b, float t)
{
	// 角度差分を求める
	float diff = b - a;

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < 2 * (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= 2 * std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	diff = std::fmod(diff, 2 * (float)std::numbers::pi);
	if (diff < (float)-std::numbers::pi)
	{
		diff += 2 * (float)std::numbers::pi;
	}
	else if (diff >= (float)std::numbers::pi)
	{
		diff -= 2 * (float)std::numbers::pi;
	}

	return Lerp(a, diff, t);
}

float DemoScene::LerpShortTranslate(float a, float b, float t) {
	return a + t * (b - a);
}

float DemoScene::Length(const Vector3& v) {
	float result;
	result = powf(v.x, 2.0) + powf(v.y, 2.0) + powf(v.z, 2.0);

	return sqrtf(result);
};