#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Loder.h"
#include "PSOPostEffect.h"

#include <DirectXMath.h>
#include "Vector3.h"  // Vector3 が定義されているヘッダーファイルをインクルード

void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	input = Input::GetInstance();

	UVtextureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	WHITEtextureHandle = TextureManager::StoreTexture("Resources/white.png");
	BLUEtextureHandle = TextureManager::StoreTexture("Resources/blue.png");
	CONEtextureHandle = TextureManager::StoreTexture("Resources/game/cone.png");
	TENQtextureHandle = TextureManager::StoreTexture("Resources/game/world2.png");
	FADEtextureHandle = TextureManager::StoreTexture("Resources/black.png");
	GRIDtextureHandle = TextureManager::StoreTexture("Resources/cian.png");
	SKYtextureHandle = TextureManager::StoreTexture("Resources/game/rostock_laage_airport_4k.dds");

	Loder::LoadJsonFile("Resources", "TL1", object3d_, camera);

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();
	skybox_ = new Skybox();
	skybox_->Init(material);
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = true;

	GridOBJ = new Object3d();
	GridOBJ->Init();
	object3d = new Object3d();
	object3d->Init();
	object3d2 = new Object3d();
	object3d2->Init();
	ConeOBJ = new Object3d();
	ConeOBJ->Init();
	TenQOBJ = new Object3d();
	TenQOBJ->Init();
	
	GridOBJ->SetMapTexture(SKYtextureHandle);
	object3d2->SetMapTexture(SKYtextureHandle);
	ConeOBJ->SetMapTexture(SKYtextureHandle);
	TenQOBJ->SetMapTexture(SKYtextureHandle);
	object3d->SetSkybox(skybox_);
	worldTransform.Initialize();
	worldTransform2.Initialize();
	worldTransformSKY.Initialize();
	GridTransform.Initialize();
	TenQTransform.Initialize();

	worldTransform.translation_.x = 0;
	worldTransform2.translation_ = { -2.5f,7.5f,82.0f };
	

	GridTransform.scale_.x = 20;
	GridTransform.scale_.z = 20;
	GridOBJ->SetWorldTransform(GridTransform);

	TenQTransform.translation_.y = 370.0f;
	TenQTransform.translation_.z = 300.0f;
	TenQTransform.scale_.x = -2.0f;
	TenQTransform.scale_.y = 2.0f;
	TenQTransform.scale_.z = 2.0f;
	TenQOBJ->SetWorldTransform(TenQTransform);

	worldTransformSKY.translation_.x = 0;
	worldTransformSKY.scale_ = { 1000,1000,1000 };
	object3d->SetWorldTransform(worldTransformSKY);

	GridOBJ->SetModel("grid.obj");
	ConeOBJ->SetModel("cone.obj");
	TenQOBJ->SetModel("world2.obj");
	object3d2->SetModel("sneakWalk.gltf");
	
	
	particle = new Particle();
	particle2 = new Particle();

	demoRandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{1.0f,4.0f}
	};

	ParticleEmitter_.count = 6;
	ParticleEmitter_.frequency = 0.02f;
	ParticleEmitter_.frequencyTime = 0.0f;
	ParticleEmitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(ParticleEmitter_);
	particle2->Initialize(ParticleEmitter_);

	fade = new Fade();
	fade->Init(FADEtextureHandle);
	fade->StartFadeOut();
}

void DemoScene::Update()
{
	// プレイヤーの座標
	Vector3 playerPos = camera->transform_.translate;
	PSOPostEffect* pSOPostEffect = PSOPostEffect::GatInstance();

	fade->UpdateFade();
	if (sceneTime == 180) {
		effect = true;
	}
	else {
		effect = false;
	}
	if (sceneTime == 360) {
		effect2 = true;
	}
	else {
		effect2 = false;
	}
	if (effect == true) {
		IPostEffectState::SetEffectNo(kOutlinePurple);
	}
	if (effect2 == true) {
		IPostEffectState::SetEffectNo(kOutlineBlue);
	}
	if (sceneTime >= 360 || sceneTime1 >= 360) {
		sceneTime = 0;
		sceneTime1 = 0;
	}
	if (input->TriggerKey(DIK_SPACE)) {
		fade->StartFadeIn();
	}
	if (fade->IsFadeOutComplete()) {
		sceneNo = 1;
	}

	TenQOBJ->worldTransform_.rotation_.x += 0.001f;
	TenQOBJ->worldTransform_.translation_.x = Lerp(TenQOBJ->worldTransform_.translation_.x, camera->transform_.translate.x, 0.005f);
	TenQOBJ->worldTransform_.translation_.y = Lerp(TenQOBJ->worldTransform_.translation_.y, camera->transform_.translate.y + 370.0f, 0.005f);
	TenQOBJ->worldTransform_.translation_.z = Lerp(TenQOBJ->worldTransform_.translation_.z, camera->transform_.translate.z + 300.0f, 0.05f);

	 // ゲームパッドの状態取得
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(joyState))
	{
		// 左スティックによる移動
		Vector3 moveLeftStick = { 0, 0, 0 };
		Vector3 move = { 0.0f, 0.0f, 0.0f };
		const float leftStickDeadZone = 0.2f;
		if (std::abs(joyState.Gamepad.sThumbLX) > leftStickDeadZone * SHRT_MAX ||
			std::abs(joyState.Gamepad.sThumbLY) > leftStickDeadZone * SHRT_MAX)
		{
			moveLeftStick = {
				(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
				0.0f,
				(float)joyState.Gamepad.sThumbLY / SHRT_MAX
			};

			float inputMagnitude = Length(moveLeftStick);
			if (inputMagnitude > leftStickDeadZone)
			{
				moveLeftStick.x *= 0.004f;
				moveLeftStick.z *= 0.004f;
			}
		}
		float desiredFOV = camera->fovY_;  // 現在のFOVを基準にする

		if (moveLeftStick.z > 0.00001f) {  // スティックがある程度前に倒されたとき
			desiredFOV = 1.4f;  // 前進時のFOV
		}
		else {
			desiredFOV = 0.8f;  // デフォルトのFOV
		}

		// 現在のFOVと目的のFOVが異なる場合のみLerpを行う
		if (camera->fovY_ != desiredFOV) {
			camera->SetFOV(Lerp(camera->fovY_, desiredFOV, 0.1f));
		}
		// カメラの向きに基づく移動方向の調整
		if (moveLeftStick.x != 0.0f || moveLeftStick.z != 0.0f)
		{
			// 移動ベクトルをカメラの向きに合わせる
			float cosY = cosf(camera->transform_.rotate.y);
			float sinY = sinf(camera->transform_.rotate.y);
			move = {
				moveLeftStick.x * cosY + moveLeftStick.z * sinY,
				0.0f,
				moveLeftStick.z * cosY - moveLeftStick.x * sinY
			};
			TenQOBJ->worldTransform_.rotation_.x += move.z;
		}
	}
	for (size_t i = 0; i < object3d_.size() - 1; i++) {
		float previousFloorHeight = playerPos.y; // 初期化しておく
		// オブジェクトの座標とサイズを取得
		Vector3 floorPos = object3d_[i]->worldTransform_.translation_;
		Vector3 floorSize = object3d_[i]->worldTransform_.scale_;
		Vector3 floorRotation = object3d_[i]->worldTransform_.rotation_;
		std::string label = "JSONmodel" + std::to_string(i);

		ImGui::Begin("OnFloorDebug");
		ImGui::Text(label.c_str());
		ImGui::Text("floor : %f %f %f", floorPos.x, floorPos.y, floorPos.z);
		ImGui::Text("size : %f %f %f", floorSize.x, floorSize.y, floorSize.z);
		ImGui::Text("isOnx : %f %f", playerPos.x, floorPos.x - floorSize.x);
		ImGui::Text("isOnx : %f %f", playerPos.x, floorPos.x + floorSize.x);
		ImGui::Text("isOnz : %f %f", playerPos.z, floorPos.z - floorSize.z);
		ImGui::Text("isOnz : %f %f", playerPos.z, floorPos.z + floorSize.z);
		ImGui::Text("isOny : %f %f", playerPos.y, abs(floorPos.y + floorSize.y + 3.0f));
		ImGui::Text("isOnyy : %f", abs(playerPos.y - (floorPos.y + floorSize.y + 3.0f)));
		ImGui::End();
		// プレイヤーがオブジェクトの上にいるか判定
		if (playerPos.x >= floorPos.x - floorSize.x&&
			playerPos.x <= floorPos.x + floorSize.x&&
			playerPos.z >= floorPos.z - floorSize.z&&
			playerPos.z <= floorPos.z + floorSize.z &&
			playerPos.y >= floorPos.y + floorSize.y - 2.0f &&
			playerPos.y <= floorPos.y + floorSize.y + 3.0f
			) {

			// 床の上昇分を計算
			float floorHeightChange = floorPos.y + floorSize.y - previousFloorHeight;
			camera->transform_.translate.y = playerPos.y += floorHeightChange + 3.0f;  // プレイヤーの高さを更新
			previousFloorHeight = floorPos.y + floorSize.y; // 次フレームのために保存

			isOnFloor = true;
			break;  // どれかのオブジェクト上にいる場合は判定を終了
		}
		else {
			isOnFloor = false;
		}
	}
	
	
		for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
			(*itr)->Update();
		}
		camera->Update();
		camera->Jump(isOnFloor);
		camera->Move();
		GridOBJ->Update();
		ConeOBJ->Update();
		TenQOBJ->Update();
		object3d2->Update();
		object3d->Update();

		object3d_[1]->worldTransform_.rotation_.x += 0.01f;
		object3d_[1]->worldTransform_.rotation_.y += 0.01f;
		object3d_[1]->worldTransform_.rotation_.z += 0.01f;
		object3d_[2]->worldTransform_.rotation_.x += 0.01f;
		object3d_[2]->worldTransform_.rotation_.y -= 0.01f;
		object3d_[2]->worldTransform_.rotation_.z -= 0.01f;
		if (sceneTime1 == 0) {
			object3d_[4]->worldTransform_.scale_.x = 5.00f;
			object3d_[4]->worldTransform_.scale_.z = 5.00f;
		}
		if (sceneTime1 < 180) {
			object3d_[4]->worldTransform_.scale_.x += 0.07f;
			object3d_[4]->worldTransform_.scale_.z += 0.02f;
			object3d_[6]->worldTransform_.translation_.y = Lerp(object3d_[6]->worldTransform_.translation_.y, 10.0f, 0.1f);
		}
		if (sceneTime1 > 180 &&sceneTime1 < 360) {
			object3d_[4]->worldTransform_.scale_.x -= 0.07f;
			object3d_[4]->worldTransform_.scale_.z -= 0.02f;
			object3d_[6]->worldTransform_.translation_.y = Lerp(object3d_[6]->worldTransform_.translation_.y, 4.0f, 0.1f);

		}
	
		if (effectFlag == true) {
			sceneTime++;
		}
		sceneTime1++;
		///////////////Debug///////////////

		camera->CameraDebug();
		// 選択されたインデックスに応じたモデルのデバッグを実行
		std::string label = "JSONmodel" + std::to_string(selectedIndex);
		object3d_[selectedIndex]->ModelDebug(label.c_str());

		GridOBJ->ModelDebug("grid");												
		ConeOBJ->ModelDebug("cone");
		TenQOBJ->ModelDebug("TenQ");
		object3d->ModelDebug("SKY");

		object3d2->ModelDebug("chara");

		particle->Particledebug("uv", worldTransform);
		particle2->Particledebug("white", worldTransform2);
		ImGui::Begin("isOnFloor");
		ImGui::SliderInt("Select Model Index", &selectedIndex, 0, static_cast<int>(object3d_.size()) - 2);
		ImGui::Text("OnFloor : %d", isOnFloor);
		ImGui::Text("Player Pos : %f %f %f", playerPos.x, playerPos.y, playerPos.z);
		ImGui::End();
		ImGui::Begin("color",nullptr,ImGuiWindowFlags_MenuBar);
		float color[4] = { fade->material.color.x,fade->material.color.y,fade->material.color.z,fade->material.color.w };
		ImGui::DragFloat4("color", color, 0.01f);
		fade->material.color = { color[0],color[1],color[2],color[3] };
		//いつか使う用に↓
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Save")) {

				}
				if (ImGui::MenuItem("Load")) {

				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
		ImGui::Begin("Space:FadeIn");
		if (ImGui::Button("FadeIn ")) {
			fade->StartFadeIn();
		}
		ImGui::Checkbox("EffectFlag", &effectFlag);
		ImGui::Text("Now Scene : %d", sceneNo);
		ImGui::End();
}

void DemoScene::Draw()
{
	for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
		(*itr)->Draw(CONEtextureHandle, camera);
	}
	//GridOBJ->Draw(GRIDtextureHandle, camera);
	//ConeOBJ->Draw(CONEtextureHandle, camera);
	TenQOBJ->Draw(TENQtextureHandle, camera);
	//object3d2->Draw(UVtextureHandle, camera);
	//object3d->Draw(SKYtextureHandle, camera);

	particle->Draw(ParticleEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	particle2->Draw(ParticleEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	fade->Draw();
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

