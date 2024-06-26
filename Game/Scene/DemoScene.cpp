#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
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
	fadeTex = TextureManager::StoreTexture("Resources/black.png");
	fadeSprite = new Sprite();

	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });

	material2.color = { 1.0f,1.0f,1.0f,0.0f };
	material2.enableLighting = true;

	uvSprite = new Sprite();
	uvSprite->Init({ 0.0f,0.0f }, { 300.0f,300.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	uvSprite->SetTextureSize({ 512.0f,512.0f });

	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = false;

	worldTransform.Initialize();
	worldTransform.translation_ = { 0,0,0 };
	worldTransform.rotation_ = { 0,0,0 };
	worldTransform.scale_ = { 1,1,1 };

	worldTransform2.Initialize();
	worldTransform2.translation_ = { 5,0,0 };
	worldTransform2.rotation_ = { 0,0,0 };
	worldTransform2.scale_ = { 1,1,1 };

	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/human", "walk.gltf");

	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();

	object3d->SetModel("sneakWalk.gltf");
	object3d2->SetModel("walk.gltf");

	particle = new Particle();
	particle2 = new Particle();

	RandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{0.0f,2.0f}
	};

	Emitter_.count = 6;
	Emitter_.frequency = 0.02f;
	Emitter_.frequencyTime = 0.0f;
	Emitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(Emitter_);
	particle2->Initialize(Emitter_);
	StartFadeOut();
}

void DemoScene::Update()
{

	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}
	if (isFadingIn == true)
	{
		UpdateFadeIn();
	}
	sceneTime++;
	////カメラの更新
	camera->Update();
	camera->CameraDebug();
	uvSprite->Update();
	fadeSprite->Update();

	object3d->SetWorldTransform(worldTransform);
	object3d2->SetWorldTransform(worldTransform2);

	Move();
	Jump();

	object3d->Update();
	object3d2->Update();

	object3d->ModelDebug("uvModel", worldTransform);
	object3d2->ModelDebug("whiteModel", worldTransform2);
	uvSprite->SpriteDebug("uvTex");
	fadeSprite->SpriteDebug("FadeTex");

	ImGui::Begin("color");
	float color[4] = { material.color.x,material.color.y,material.color.z,material.color.w };
	ImGui::DragFloat4("uvTex", color, 0.01f);
	material.color = { color[0],color[1],color[2],color[3] };
	float color1[4] = { material2.color.x,material2.color.y,material2.color.z,material2.color.w };
	ImGui::DragFloat4("FadeTex", color1, 0.01f);
	material2.color = { color1[0],color1[1],color1[2],color1[3] };
	ImGui::End();
	ImGui::Begin("read me");
	ImGui::ShowStyleSelector("a");
	ImGui::Text("move : WASD or Joystick");
	ImGui::Text("jump : SPACE or A button");
	static char buf[99] = "hoge";
	ImGui::Text("%s", buf);
	ImGui::InputText("string", buf, sizeof(buf));
	ImGui::Checkbox("FadeIn", &isFadingIn);
	ImGui::End();
	postProcess_->Update();
}
void DemoScene::Draw()
{
	uvSprite->Draw(textureHandle, material.color);
	object3d->Draw(textureHandle, camera);
	object3d2->Draw(textureHandle2, camera);
	/*particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, demoRandPro, false);*/
	fadeSprite->Draw(fadeTex, material2.color);
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
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		move.z = PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		move.z = -PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	if (Input::GetInstance()->PushKey(DIK_A))
	{
		move.x = -PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		move.x = PlayerSpeed;
		//目標角度の算出
		angle_ = std::atan2(move.x, move.z);
	}

	// Y軸周り角度(θy)	歩いている方向に顔を向ける
	worldTransform.rotation_.y = LerpShortAngle(worldTransform.rotation_.y, angle_, 0.1f);
	worldTransform.translation_.x += move.x;
	worldTransform.translation_.z += move.z;

	if (Input::GetInstance()->GetJoystickState(0, joyState))
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
	worldTransform.UpdateMatrix();
}

void DemoScene::Jump()
{
	XINPUT_STATE joyState;
	//ゲームパットの状態を得る変数(XINPUT)
	Vector3 move = { 0.0f,0.0f,0.0f };
	//移動
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && flag == false)
	{
		flag = true;
	}
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		{
			flag = true;
		}
	}
	if (flag == true) {
		flagcount++;
		move.y = PlayerJumpSpeed;
		PlayerJumpSpeed -= 0.005f;
	}
	else {
		worldTransform.translation_.y = 0.000f;
	}
	if (PlayerJumpSpeed <= -0.08f) {
		flag = false;
		PlayerJumpSpeed = 0.08f;
	}

	worldTransform.translation_.y += move.y;
	worldTransform.UpdateMatrix();
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

void DemoScene::StartFadeOut()
{
	isFadeOut = true;
	alpha = 1.0f;
	material2.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void DemoScene::UpdateFadeOut()
{
	alpha -= 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material2.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha <= 0.0f)
	{
		// フェードイン完了時の処理
		isFadeOut = false;
	}
}


void DemoScene::StartFadeIn()
{
	isFadingIn = true;
	alpha = 0.0f;
	material2.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void DemoScene::UpdateFadeIn()
{
	alpha += 0.01f; // フェードイン速度の調整（必要に応じて変更）
	material2.color = { 1.0f, 1.0f, 1.0f, alpha };

	if (alpha >= 1.0f)
	{
		// フェードイン完了時の処理
		isFadingIn = false;
		alpha = 0.0f;
		sceneNo = 0;
	}
}