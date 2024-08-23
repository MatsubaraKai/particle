#include "TitleScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include <iostream>
#include <cmath>
#include "mathFunction.h"
#include "PSOPostEffect.h"
#include "Loder.h"
#include "PSOPostEffect.h"

#include <DirectXMath.h>
#include "Vector3.h"

void TitleScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	input = Input::GetInstance();
	WHITEtextureHandle = TextureManager::StoreTexture("Resources/white.png");
	BLUEtextureHandle = TextureManager::StoreTexture("Resources/blue.png");
	CONEtextureHandle = TextureManager::StoreTexture("Resources/game/cone.png");
	FADEtextureHandle = TextureManager::StoreTexture("Resources/black.png");
	GRIDtextureHandle = TextureManager::StoreTexture("Resources/cian.png");

	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadAnimationModel("Resources/AnimatedCube", "AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/game", "grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "cone.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "world2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text3.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text4.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text5.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text6.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "0.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "1.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "3.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "4.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "5.obj");

	if (GameRoop == false) {
		Loder::LoadJsonFile2("Resources", "TitleCone", ConeObject_);
	}
	
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();
	
	isFadeInStarted = false;

	worldTransformPa.Initialize();
	worldTransformPa.translation_ = { -2.5f,7.5f,60.0f };//チュートリアルポータル
	worldTransformPa1.Initialize();
	worldTransformPa1.translation_ = { -2.5f,7.5f,82.0f };//ゲームシーンポータル
	worldTransformPa2.Initialize();
	worldTransformPa2.translation_ = { -2.5f,7.5f,82.0f };//ゲームシーン２ポータル

	camera->transform_.translate = { 0.0f,15.0f,-15.0f };

	particle = new Particle();
	particle1 = new Particle();
	particle2 = new Particle();
	particle3 = new Particle();
	demoRandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{1.0f,4.0f}
	};
	isClear = false;

	ParticleEmitter_.count = 6;
	ParticleEmitter_.frequency = 0.02f;
	ParticleEmitter_.frequencyTime = 0.0f;
	ParticleEmitter_.transform.scale = { 0.5f,0.5f,0.5f };
	particle->Initialize(ParticleEmitter_);
	particle1->Initialize(ParticleEmitter_);
	particle2->Initialize(ParticleEmitter_);
	particle3->Initialize(ParticleEmitter_);
	fade = new Fade();
	fade->Init(FADEtextureHandle);
	fade->StartFadeOut();
}

void TitleScene::Update()
{
	fade->UpdateFade();
	PSOPostEffect* pSOPostEffect = PSOPostEffect::GatInstance();
	// プレイヤーの座標
	Vector3 playerPos = camera->transform_.translate;
	Vector3 particlePos = worldTransformPa.translation_;
	Vector3 particlePos1 = worldTransformPa1.translation_;
	Vector3 particlePos2 = worldTransformPa2.translation_;
	// パーティクルとカメラの距離を計算
	float dx = (particlePos.x + 2.5f) - playerPos.x;
	float dy = (particlePos.y + 2.5f) - playerPos.y;
	float dz = (particlePos.z + 2.5f) - playerPos.z;

	float dx1 = (particlePos1.x + 2.5f) - playerPos.x;
	float dy1 = (particlePos1.y + 2.5f) - playerPos.y;
	float dz1 = (particlePos1.z + 2.5f) - playerPos.z;

	float dx2 = (particlePos2.x + 2.5f) - playerPos.x;
	float dy2 = (particlePos2.y + 2.5f) - playerPos.y;
	float dz2 = (particlePos2.z + 2.5f) - playerPos.z;

	float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
	float distance1 = std::sqrt(dx1 * dx1 + dy1 * dy1 + dz1 * dz1);
	float distance2 = std::sqrt(dx2 * dx2 + dy2 * dy2 + dz2 * dz2);
	// 衝突判定
	float collisionDistance = 2.0f; // 任意の衝突距離 (調整可能)

	if (distance < collisionDistance) {
		// 衝突している
		isDemo = true;
		isClear = true;
	}
	else {
		isDemo = false;
	}
	if (distance1 < collisionDistance) {
		// 衝突している
		isGame = true;
		isClear = true;
	}
	else {
		isGame = false;
	}
	if (distance2 < collisionDistance) {
		// 衝突している
		isGame2 = true;
		isClear = true;
	}
	else {
		isGame2 = false;
	}
	if (sceneTime == 180) {
		effect = true;
	}else {
		effect = false;
	}
	if (sceneTime == 360) {
		effect2 = true;
	}else {
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
	if (fade->IsFadeOutComplete() && isDemo) {
		sceneNo = 3;
	}
	if (fade->IsFadeOutComplete() && isGame) {
		sceneNo = 1;
	}
	if (fade->IsFadeOutComplete() && isGame2) {
		sceneNo = 2;
	}
	
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
	}
	for (size_t i = 0; i < ConeObject_.size() - 1; i++) {
		if (ConeObject_[i]->isVisible) {
			float previousFloorHeight = playerPos.y; // 初期化しておく
			// オブジェクトの座標とサイズを取得
			Vector3 floorPos = ConeObject_[i]->worldTransform_.translation_;
			Vector3 floorSize = ConeObject_[i]->worldTransform_.scale_;
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
			if (playerPos.x >= floorPos.x - floorSize.x &&
				playerPos.x <= floorPos.x + floorSize.x &&
				playerPos.z >= floorPos.z - floorSize.z &&
				playerPos.z <= floorPos.z + floorSize.z &&
				playerPos.y >= floorPos.y + floorSize.y - 1.0f &&
				playerPos.y <= floorPos.y + floorSize.y + 3.0f) {

				// 床の上昇分を計算
				float floorHeightChange = floorPos.y + floorSize.y - previousFloorHeight;
				camera->transform_.translate.y = playerPos.y += floorHeightChange + 3.0f;  // プレイヤーの高さを更新
				previousFloorHeight = floorPos.y + floorSize.y; // 次フレームのために保存

				isOnFloor = true;
				//ConeObject_[i]->isVisible = false;  // 該当オブジェクトの描画を停止
				break;  // どれかのオブジェクト上にいる場合は判定を終了
			}
			else {
				isOnFloor = false;
			}
		}
		else {
			isOnFloor = false;
		}
	}

	for (std::vector<Object3d*>::iterator itr1 = ConeObject_.begin(); itr1 != ConeObject_.end(); itr1++) {
		if ((*itr1)->isVisible) {
			(*itr1)->Update();
		}
	}
	if (isClear == false) {
		camera->Jump(isOnFloor);
		camera->Move();
	}else {
		if (!isFadeInStarted) {
			fade->StartFadeIn();    // FadeInを開始
			isFadeInStarted = true; // フラグを立てて一度だけ実行されるようにする
		}
	}
	camera->Update();

	if (sceneTime1 == 0) {

	}
	if (sceneTime1 < 180) {

	}
	if (sceneTime1 > 180 && sceneTime1 < 360) {

	}
	if (effectFlag == true) {
		sceneTime++;
	}
	sceneTime1++;
	///////////////Debug///////////////

	camera->CameraDebug();
	// 選択されたインデックスに応じたモデルのデバッグを実行
	std::string label1 = "JSONConemodel" + std::to_string(selectedIndex1);
	ConeObject_[selectedIndex1]->ModelDebug(label1.c_str());

	particle->Particledebug("white", worldTransformPa);
	particle1->Particledebug("white1", worldTransformPa1);
	particle2->Particledebug("white2", worldTransformPa2);
	ImGui::Begin("isOnFloor");
	ImGui::SliderInt("Select Model Index", &selectedIndex1, 0, static_cast<int>(ConeObject_.size()) - 2);
	ImGui::Text("OnFloor : %d", isOnFloor);
	ImGui::Text("Player Pos : %f %f %f", playerPos.x, playerPos.y, playerPos.z);
	ImGui::End();
	ImGui::Begin("color", nullptr, ImGuiWindowFlags_MenuBar);
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
	ImGui::Text("roop : %d", GameRoop);
	ImGui::End();
	
}
void TitleScene::Draw()
{
	for (std::vector<Object3d*>::iterator itr1 = ConeObject_.begin(); itr1 != ConeObject_.end(); itr1++) {
		if ((*itr1)->isVisible) {
			(*itr1)->Draw(CONEtextureHandle, camera);

		}
	}
	particle->Draw(ParticleEmitter_, { worldTransformPa.translation_.x,worldTransformPa.translation_.y,worldTransformPa.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	particle1->Draw(ParticleEmitter_, { worldTransformPa1.translation_.x,worldTransformPa1.translation_.y,worldTransformPa1.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	particle2->Draw(ParticleEmitter_, { worldTransformPa2.translation_.x,worldTransformPa2.translation_.y,worldTransformPa2.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	fade->Draw();
}

void TitleScene::PostDraw()
{
	postProcess_->Draw();
}

void TitleScene::Release() {

}

// ゲームを終了
int TitleScene::GameClose()
{
	return false;
}
