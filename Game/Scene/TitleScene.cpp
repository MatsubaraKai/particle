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
	FADEtextureHandle = TextureManager::StoreTexture("Resources/black.png");
	GRIDtextureHandle = TextureManager::StoreTexture("Resources/cian.png");
	CONEtextureHandle = TextureManager::StoreTexture("Resources/game/cone.png");
	TENQtextureHandle = TextureManager::StoreTexture("Resources/game/world.png");

	ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "world2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text3.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text4.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text5.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text6.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "text7.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext3.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext4.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext5.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext6.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext7.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Text", "Titletext8.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "0.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "1.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "2.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "3.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "4.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game/Number", "5.obj");

	if (TitleRoop == false) {
		Loder::LoadJsonFile2("Resources", "TitleCone", ConeObject_);
		TitleRoop = true;
	}
	
	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	TenQOBJ = new Object3d();
	TenQOBJ->Init();
	TitleOBJ = new Object3d();
	TitleOBJ->Init();
	TitleOBJ2 = new Object3d();
	TitleOBJ2->Init();
	TextOBJ3 = new Object3d();
	TextOBJ3->Init();
	TextOBJ4 = new Object3d();
	TextOBJ4->Init();
	TextOBJ5 = new Object3d();
	TextOBJ5->Init();
	TextOBJ6 = new Object3d();
	TextOBJ6->Init();
	TextOBJ7 = new Object3d();
	TextOBJ7->Init();
	TextOBJ8 = new Object3d();
	TextOBJ8->Init();

	isFadeInStarted = false;

	TenQTransform.Initialize();

	TenQTransform.scale_.x = -100.0f;
	TenQTransform.scale_.y = 100.0f;
	TenQTransform.scale_.z = 100.0f;
	TenQOBJ->SetWorldTransform(TenQTransform);


	TitleOBJ->worldTransform_.translation_ = { -12.0f,22.5f,30.0f };
	TitleOBJ->worldTransform_.rotation_.x = -0.2f;
	TitleOBJ->worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	TitleOBJ2->worldTransform_.translation_ = { 12.0f,10.0f,30.0f };
	TitleOBJ2->worldTransform_.scale_ = { 20.0f,20.0f,20.0f };
	TextOBJ3->worldTransform_.translation_ = { -22.5f,7.0f,15.0f };
	TextOBJ4->worldTransform_.translation_ = { 27.5f,7.0f,15.0f };
	TextOBJ5->worldTransform_.translation_ = { 27.5f,7.0f,2.5f };
	TextOBJ6->worldTransform_.translation_ = { 27.5f,7.0f,-10.0f };
	TextOBJ8->worldTransform_.translation_ = { -22.5f,2.0f,12.5f };

	worldTransformPa.Initialize();
	worldTransformPa.translation_ = { -25.0f,1.5f,12.5f };//チュートリアルポータル
	worldTransformPa1.Initialize();
	worldTransformPa1.translation_ = { 25.0f,1.5f,12.5f };//ゲームシーンポータル
	worldTransformPa2.Initialize();
	worldTransformPa2.translation_ = { 25.0f,1.5f,0.0f };//ゲームシーン２ポータル
	worldTransformPa3.Initialize();
	worldTransformPa3.translation_ = { 25.0f,1.5f,-12.5f };//ゲームシーン３ポータル

	camera->transform_.translate = { 0.0f,15.0f,-15.0f };
	camera->transform_.rotate = { -0.2f, 0.0f, 0.0f };

	TenQOBJ->SetModel("world.obj");
	TitleOBJ->SetModel("Titletext.obj");
	TitleOBJ2->SetModel("Titletext2.obj");
	TextOBJ3->SetModel("Titletext3.obj");
	TextOBJ4->SetModel("Titletext4.obj");
	TextOBJ5->SetModel("Titletext5.obj");
	TextOBJ6->SetModel("Titletext6.obj");
	TextOBJ7->SetModel("Titletext7.obj");
	TextOBJ8->SetModel("Titletext8.obj");

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
	Vector3 particlePos3 = worldTransformPa3.translation_;
	// パーティクルとカメラの距離を計算
	float dx = (particlePos.x + 2.5f) - playerPos.x;
	float dy = (particlePos.y + 4.0f) - playerPos.y;
	float dz = (particlePos.z + 2.5f) - playerPos.z;

	float dx1 = (particlePos1.x + 2.5f) - playerPos.x;
	float dy1 = (particlePos1.y + 4.0f) - playerPos.y;
	float dz1 = (particlePos1.z + 2.5f) - playerPos.z;

	float dx2 = (particlePos2.x + 2.5f) - playerPos.x;
	float dy2 = (particlePos2.y + 4.0f) - playerPos.y;
	float dz2 = (particlePos2.z + 2.5f) - playerPos.z;

	float dx3 = (particlePos3.x + 2.5f) - playerPos.x;
	float dy3 = (particlePos3.y + 4.0f) - playerPos.y;
	float dz3 = (particlePos3.z + 2.5f) - playerPos.z;

	float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
	float distance1 = std::sqrt(dx1 * dx1 + dy1 * dy1 + dz1 * dz1);
	float distance2 = std::sqrt(dx2 * dx2 + dy2 * dy2 + dz2 * dz2);
	float distance3 = std::sqrt(dx3 * dx3 + dy3 * dy3 + dz3 * dz3);
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
	if (distance3 < collisionDistance) {
		// 衝突している
		isGame3 = true;
		isClear = true;
	}
	else {
		isGame3 = false;
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
		sceneNo = 1;
	}
	if (fade->IsFadeOutComplete() && isGame) {
		sceneNo = 2;
	}
	if (fade->IsFadeOutComplete() && isGame2) {
		sceneNo = 3;
	}
	if (fade->IsFadeOutComplete() && isGame3) {
		sceneNo = 4;
	}
	TenQOBJ->worldTransform_.rotation_.y += 0.0005f;
	TitleOBJ->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TitleOBJ->worldTransform_.translation_) + 3.14f;
	TitleOBJ2->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TitleOBJ2->worldTransform_.translation_) + 3.14f;
	TextOBJ3->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ3->worldTransform_.translation_) + 3.14f;
	TextOBJ4->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ4->worldTransform_.translation_) + 3.14f;
	TextOBJ5->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ5->worldTransform_.translation_) + 3.14f;
	TextOBJ6->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ6->worldTransform_.translation_) + 3.14f;
	TextOBJ7->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ7->worldTransform_.translation_) + 3.14f;
	TextOBJ8->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ8->worldTransform_.translation_) + 3.14f;

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
				break;  // どれかのオブジェクト上にいる場合は判定を終了
			}
			else {
				isOnFloor = false;
			}
	}

	for (std::vector<Object3d*>::iterator itr1 = ConeObject_.begin(); itr1 != ConeObject_.end(); itr1++) {
		(*itr1)->Update();
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
	TenQOBJ->Update();
	TitleOBJ->Update();
	TitleOBJ2->Update();
	TextOBJ3->Update();
	TextOBJ4->Update();
	TextOBJ5->Update();
	TextOBJ6->Update();
	TextOBJ7->Update();
	TextOBJ8->Update();

	if (playerPos.x >= -20.0f &&
		playerPos.x <= 20.0f &&
		playerPos.z >= -20.0f &&
		playerPos.z <= 20.0f && DemoRoop == false
		) {
		TextOBJ7->worldTransform_.translation_.y = Lerp(TextOBJ7->worldTransform_.translation_.y, 1.3f, 0.1f);
	}else {
		TextOBJ7->worldTransform_.translation_.y = Lerp(TextOBJ7->worldTransform_.translation_.y, 0.0f, 0.1f);
	}
	if (DemoRoop == false) {
		TextOBJ8->worldTransform_.translation_.y = Lerp(TextOBJ8->worldTransform_.translation_.y, 2.0f, 0.1f);
	}
	else {
		TextOBJ8->worldTransform_.translation_.y = Lerp(TextOBJ8->worldTransform_.translation_.y, 0.0f, 0.1f);
	}
	if (sceneTime1 == 0) {

	}
	if (sceneTime1 < 180) {
		TitleOBJ->worldTransform_.translation_.y = Lerp(TitleOBJ->worldTransform_.translation_.y, 25.0f, 0.01f);
		TitleOBJ2->worldTransform_.translation_.y = Lerp(TitleOBJ2->worldTransform_.translation_.y, 12.5f, 0.01f);
		TextOBJ3->worldTransform_.translation_.y = Lerp(TextOBJ3->worldTransform_.translation_.y, 7.5f, 0.01f);
		TextOBJ4->worldTransform_.translation_.y = Lerp(TextOBJ4->worldTransform_.translation_.y, 7.5f, 0.01f);
		TextOBJ5->worldTransform_.translation_.y = Lerp(TextOBJ5->worldTransform_.translation_.y, 7.5f, 0.01f);
		TextOBJ6->worldTransform_.translation_.y = Lerp(TextOBJ6->worldTransform_.translation_.y, 7.5f, 0.01f);

	}
	if (sceneTime1 > 180 && sceneTime1 < 360) {
		TitleOBJ->worldTransform_.translation_.y = Lerp(TitleOBJ->worldTransform_.translation_.y, 20.0f, 0.01f);
		TitleOBJ2->worldTransform_.translation_.y = Lerp(TitleOBJ2->worldTransform_.translation_.y, 7.5f, 0.01f);
		TextOBJ3->worldTransform_.translation_.y = Lerp(TextOBJ3->worldTransform_.translation_.y, 6.5f, 0.01f);
		TextOBJ4->worldTransform_.translation_.y = Lerp(TextOBJ4->worldTransform_.translation_.y, 6.5f, 0.01f);
		TextOBJ5->worldTransform_.translation_.y = Lerp(TextOBJ5->worldTransform_.translation_.y, 6.5f, 0.01f);
		TextOBJ6->worldTransform_.translation_.y = Lerp(TextOBJ6->worldTransform_.translation_.y, 6.5f, 0.01f);

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

	TenQOBJ->ModelDebug("TenQ");
	TitleOBJ->ModelDebug("text");
	TitleOBJ2->ModelDebug("text2");
	TextOBJ3->ModelDebug("text3");
	TextOBJ4->ModelDebug("text4");
	TextOBJ5->ModelDebug("text5");
	TextOBJ6->ModelDebug("text6");
	TextOBJ7->ModelDebug("text7");
	TextOBJ8->ModelDebug("text8");
	particle->Particledebug("white", worldTransformPa);
	particle1->Particledebug("white1", worldTransformPa1);
	particle2->Particledebug("white2", worldTransformPa2);
	particle3->Particledebug("white3", worldTransformPa3);
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
	ImGui::Text("roop : %d", TitleRoop);
	ImGui::End();
	
}
void TitleScene::Draw()
{
	for (std::vector<Object3d*>::iterator itr1 = ConeObject_.begin(); itr1 != ConeObject_.end(); itr1++) {
		if ((*itr1)->isVisible) {
			(*itr1)->Draw(CONEtextureHandle, camera);

		}
	}
	TenQOBJ->Draw(TENQtextureHandle, camera);
	TitleOBJ->Draw(BLUEtextureHandle, camera);
	TitleOBJ2->Draw(BLUEtextureHandle, camera);
	TextOBJ3->Draw(GRIDtextureHandle, camera);
	TextOBJ4->Draw(GRIDtextureHandle, camera);
	TextOBJ5->Draw(GRIDtextureHandle, camera);
	TextOBJ6->Draw(GRIDtextureHandle, camera);
	TextOBJ7->Draw(GRIDtextureHandle, camera);
	TextOBJ8->Draw(GRIDtextureHandle, camera);
	particle->Draw(ParticleEmitter_, { worldTransformPa.translation_.x,worldTransformPa.translation_.y,worldTransformPa.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	particle1->Draw(ParticleEmitter_, { worldTransformPa1.translation_.x,worldTransformPa1.translation_.y,worldTransformPa1.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	particle2->Draw(ParticleEmitter_, { worldTransformPa2.translation_.x,worldTransformPa2.translation_.y,worldTransformPa2.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
	particle3->Draw(ParticleEmitter_, { worldTransformPa3.translation_.x,worldTransformPa3.translation_.y,worldTransformPa3.translation_.z }, WHITEtextureHandle, camera, demoRandPro, false);
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
