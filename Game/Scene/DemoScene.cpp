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
	STARtextureHandle = TextureManager::StoreTexture("Resources/game/star.png");
	SKYtextureHandle = TextureManager::StoreTexture("Resources/game/rostock_laage_airport_4k.dds");

	Loder::LoadJsonFile("Resources", "DemoCone", ConeObject_, camera);
	Loder::LoadJsonFile2("Resources", "DemoStar", StarObject_);

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
	TextOBJ = new Object3d();
	TextOBJ->Init();
	TextOBJ2 = new Object3d();
	TextOBJ2->Init();
	TextOBJ3 = new Object3d();
	TextOBJ3->Init();
	TextOBJ4 = new Object3d();
	TextOBJ4->Init();
	TextOBJ5 = new Object3d();
	TextOBJ5->Init();
	Number = new Object3d();
	Number->Init();

	object3d->SetSkybox(skybox_);
	worldTransform.Initialize();
	worldTransform2.Initialize();
	worldTransformSKY.Initialize();
	GridTransform.Initialize();
	TenQTransform.Initialize();

	worldTransform.translation_ = { -2.5f,1.5f,-32.35f };
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

	TextOBJ->worldTransform_.translation_.y = 7.0f;
	TextOBJ2->worldTransform_.translation_.y = 8.11f;
	TextOBJ2->worldTransform_.translation_.z = -30.0f;
	TextOBJ3->worldTransform_.translation_ = { -10.0f,4.0f,-30.0f };
	TextOBJ4->worldTransform_.translation_ = { 10.0f,4.0f,-30.0f };
	TextOBJ5->worldTransform_.translation_.y = 0.0f;
	TextOBJ5->worldTransform_.translation_.z = -30.0f;
	TextOBJ5->worldTransform_.rotation_.x = 0.43f;
	Number->worldTransform_.translation_ = { 0.0f,13.0f,84.5f };
	Number->worldTransform_.scale_ = { 2.0f,2.0f,2.0f };

	worldTransformSKY.translation_.x = 0;
	worldTransformSKY.scale_ = { 1000,1000,1000 };
	object3d->SetWorldTransform(worldTransformSKY);

	GridOBJ->SetModel("grid.obj");
	ConeOBJ->SetModel("cone.obj");
	TenQOBJ->SetModel("world2.obj");
	TextOBJ->SetModel("text.obj");
	TextOBJ2->SetModel("text2.obj");
	TextOBJ3->SetModel("text3.obj");
	TextOBJ4->SetModel("text4.obj");
	TextOBJ5->SetModel("text5.obj");
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
	std::string modelFileName = std::to_string(starCount) + ".obj";
	Number->SetModel(modelFileName.c_str());
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

	TextOBJ->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ->worldTransform_.translation_) + 3.14f;
	TextOBJ2->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ2->worldTransform_.translation_) + 3.14f;
	TextOBJ3->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ3->worldTransform_.translation_) + 3.14f;
	TextOBJ4->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ4->worldTransform_.translation_) + 3.14f;
	TextOBJ5->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, TextOBJ5->worldTransform_.translation_) + 3.14f;
	Number->worldTransform_.rotation_.y = camera->Face2Face(camera->transform_.translate, Number->worldTransform_.translation_) + 3.14f;
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

	for (size_t i = 0; i < StarObject_.size() - 1; i++) {
		if (StarObject_[i]->isVisible) {
			// オブジェクトの座標とサイズを取得
			Vector3 floorPos = StarObject_[i]->worldTransform_.translation_;
			Vector3 floorSize = StarObject_[i]->worldTransform_.scale_;

			// プレイヤーがオブジェクトに当たっているか判定
			if (playerPos.x >= floorPos.x - floorSize.x &&
				playerPos.x <= floorPos.x + floorSize.x &&
				playerPos.z >= floorPos.z - floorSize.z &&
				playerPos.z <= floorPos.z + floorSize.z &&
				playerPos.y >= floorPos.y + floorSize.y - 3.0f &&
				playerPos.y <= floorPos.y + floorSize.y + 3.0f) {
				isGetStar = true;
				StarObject_[i]->isVisible = false;  // 該当オブジェクトの描画を停止
				break;  // 判定を終了
			}
			else {
				isGetStar = false;
			}
		}
		else {
			isGetStar = false;
		}
	}
	if (isGetStar) {
		starCount--;

	}
	for (std::vector<Object3d*>::iterator itr1 = ConeObject_.begin(); itr1 != ConeObject_.end(); itr1++) {
		if ((*itr1)->isVisible) {
			(*itr1)->Update();
		}
	}
	for (std::vector<Object3d*>::iterator itr2 = StarObject_.begin(); itr2 != StarObject_.end(); itr2++) {
		if ((*itr2)->isVisible) {
			(*itr2)->Update();
			(*itr2)->worldTransform_.rotation_.y += 0.02f;
		}
	}
		camera->Update();
		camera->Jump(isOnFloor);
		camera->Move();
		GridOBJ->Update();
		ConeOBJ->Update();
		TenQOBJ->Update();
		TextOBJ->Update();
		TextOBJ2->Update();
		TextOBJ3->Update();
		TextOBJ4->Update();
		TextOBJ5->Update();
		Number->Update();
		object3d2->Update();
		object3d->Update();

		ConeObject_[1]->worldTransform_.rotation_.x += 0.01f;
		ConeObject_[1]->worldTransform_.rotation_.y += 0.01f;
		ConeObject_[1]->worldTransform_.rotation_.z += 0.01f;
		ConeObject_[2]->worldTransform_.rotation_.x += 0.01f;
		ConeObject_[2]->worldTransform_.rotation_.y -= 0.01f;
		ConeObject_[2]->worldTransform_.rotation_.z -= 0.01f;
		if (sceneTime1 == 0) {
			ConeObject_[4]->worldTransform_.scale_.x = 5.00f;
			ConeObject_[4]->worldTransform_.scale_.z = 5.00f;
		}
		if (sceneTime1 < 180) {
			ConeObject_[4]->worldTransform_.scale_.x += 0.06f;
			ConeObject_[4]->worldTransform_.scale_.z += 0.02f;
			ConeObject_[6]->worldTransform_.translation_.y = Lerp(ConeObject_[6]->worldTransform_.translation_.y, 30.0f, 0.1f);
			TextOBJ->worldTransform_.translation_.y = Lerp(TextOBJ->worldTransform_.translation_.y, 8.00f, 0.01f);
			TextOBJ2->worldTransform_.translation_.y = Lerp(TextOBJ2->worldTransform_.translation_.y, 8.61f, 0.01f);
			TextOBJ3->worldTransform_.translation_.y = Lerp(TextOBJ3->worldTransform_.translation_.y, 4.5f, 0.01f);
			TextOBJ4->worldTransform_.translation_.y = Lerp(TextOBJ4->worldTransform_.translation_.y, 4.5f, 0.01f);
			TextOBJ5->worldTransform_.translation_.y = Lerp(TextOBJ5->worldTransform_.translation_.y, 0.5f, 0.01f);
		}
		if (sceneTime1 > 180 &&sceneTime1 < 360) {
			ConeObject_[4]->worldTransform_.scale_.x -= 0.06f;
			ConeObject_[4]->worldTransform_.scale_.z -= 0.02f;
			ConeObject_[6]->worldTransform_.translation_.y = Lerp(ConeObject_[6]->worldTransform_.translation_.y, 4.0f, 0.1f);
			TextOBJ->worldTransform_.translation_.y = Lerp(TextOBJ->worldTransform_.translation_.y, 6.00f, 0.01f);
			TextOBJ2->worldTransform_.translation_.y = Lerp(TextOBJ2->worldTransform_.translation_.y, 7.61f, 0.01f);
			TextOBJ3->worldTransform_.translation_.y = Lerp(TextOBJ3->worldTransform_.translation_.y, 3.5f, 0.01f);
			TextOBJ4->worldTransform_.translation_.y = Lerp(TextOBJ4->worldTransform_.translation_.y, 3.5f, 0.01f);
			TextOBJ5->worldTransform_.translation_.y = Lerp(TextOBJ5->worldTransform_.translation_.y, -0.5f, 0.01f);
		}
	
		if (effectFlag == true) {
			sceneTime++;
		}
		sceneTime1++;
		///////////////Debug///////////////

		camera->CameraDebug();
		// 選択されたインデックスに応じたモデルのデバッグを実行
		std::string label1 = "JSONConemodel" + std::to_string(selectedIndex1);
		std::string label2 = "JSONStarmodel" + std::to_string(selectedIndex2);
		ConeObject_[selectedIndex1]->ModelDebug(label1.c_str());
		StarObject_[selectedIndex2]->ModelDebug(label2.c_str());

		GridOBJ->ModelDebug("grid");												
		ConeOBJ->ModelDebug("cone");
		TenQOBJ->ModelDebug("TenQ");
		TextOBJ->ModelDebug("text");
		TextOBJ2->ModelDebug("text2");
		TextOBJ3->ModelDebug("text3");
		TextOBJ4->ModelDebug("text4");
		TextOBJ5->ModelDebug("text5");
		Number->ModelDebug("num");
		object3d->ModelDebug("SKY");
		object3d2->ModelDebug("chara");

		particle->Particledebug("uv", worldTransform);
		particle2->Particledebug("white", worldTransform2);
		ImGui::Begin("isOnFloor");
		ImGui::SliderInt("Select Model Index", &selectedIndex1, 0, static_cast<int>(ConeObject_.size()) - 2);
		ImGui::SliderInt("Select Model Index", &selectedIndex2, 0, static_cast<int>(StarObject_.size()) - 2);
		ImGui::Text("starcount: %d", starCount);
		ImGui::Text("OnFloor : %d", isOnFloor);
		ImGui::Text("GetStar : %d", isGetStar);
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
	for (std::vector<Object3d*>::iterator itr1 = ConeObject_.begin(); itr1 != ConeObject_.end(); itr1++) {
		if ((*itr1)->isVisible) {
			(*itr1)->Draw(CONEtextureHandle, camera);

		}
	}
	for (std::vector<Object3d*>::iterator itr2 = StarObject_.begin(); itr2 != StarObject_.end(); itr2++) {
		if ((*itr2)->isVisible) {
			(*itr2)->Draw(STARtextureHandle, camera);
		}
	}
	//GridOBJ->Draw(GRIDtextureHandle, camera);
	//ConeOBJ->Draw(CONEtextureHandle, camera);
	TenQOBJ->Draw(TENQtextureHandle, camera);
	TextOBJ->Draw(GRIDtextureHandle, camera);
	TextOBJ2->Draw(GRIDtextureHandle, camera);
	TextOBJ3->Draw(GRIDtextureHandle, camera);
	TextOBJ4->Draw(GRIDtextureHandle, camera);
	TextOBJ5->Draw(GRIDtextureHandle, camera);
	Number->Draw(GRIDtextureHandle, camera);
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

