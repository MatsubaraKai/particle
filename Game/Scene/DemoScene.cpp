#include "DemoScene.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Loder.h"
#include "PSOPostEffect.h"
void DemoScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	Vector3 cameraPos = camera->GetTransform().translate;
	cameraPos.y = 3.0f;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();

	UVtextureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	WHITEtextureHandle = TextureManager::StoreTexture("Resources/white.png");
	CONEtextureHandle = TextureManager::StoreTexture("Resources/game/cone.png");
	TENQtextureHandle = TextureManager::StoreTexture("Resources/game/world2.png");
	FADEtextureHandle = TextureManager::StoreTexture("Resources/black.png");
	GRIDtextureHandle = TextureManager::StoreTexture("Resources/cian.png");
	SKYtextureHandle = TextureManager::StoreTexture("Resources/game/rostock_laage_airport_4k.dds");

	Loder::LoadJsonFile("Resources", "TL", object3d_, camera);

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();
	skybox_ = new Skybox();
	skybox_->Init(material);
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = true;

	GridOBJ = new Object3d();
	GridOBJ->Init();
	SkyBoxOBJ = new Object3d();
	SkyBoxOBJ->Init();
	PlayerOBJ = new Object3d();
	PlayerOBJ->Init();
	ConeOBJ = new Object3d();
	ConeOBJ->Init();
	TenQOBJ = new Object3d();
	TenQOBJ->Init();
	
	SkyBoxOBJ->SetSkybox(skybox_);
	worldTransform.Initialize();
	worldTransform2.Initialize();
	worldTransformSKY.Initialize();
	GridTransform.Initialize();
	TenQTransform.Initialize();

	worldTransform.translation_.x = 0;
	worldTransform2.translation_.x = 5;

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
	SkyBoxOBJ->SetWorldTransform(worldTransformSKY);

	PlayerOBJ->worldTransform_.translation_ = { -2.12f,0.0f,-8.98f };
	PlayerOBJ->worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
	PlayerOBJ->worldTransform_.rotation_ = { 0.0f,2.93f,0.0f };

	GridOBJ->SetModel("grid.obj");
	ConeOBJ->SetModel("cone.obj");
	TenQOBJ->SetModel("world2.obj");
	PlayerOBJ->SetModel("sneakWalk.gltf");
	
	
	particle = new Particle();
	particle2 = new Particle();

	demoRandPro = {
		{1.0f,4.0f},
		{1.0f,4.0f},
		{0.0f,2.0f}
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
	PSOPostEffect* pSOPostEffect = PSOPostEffect::GatInstance();

	fade->UpdateFade();
	//if (sceneTime == 180) {
	//	effect = true;
	//}
	//else {
	//	effect = false;
	//}
	//if (sceneTime == 360) {
	//	effect2 = true;
	//}
	//else {
	//	effect2 = false;
	//}
	//if (effect == true) {
	//	IPostEffectState::SetEffectNo(kOutlinePurple);
	//}
	//if (effect2 == true) {
	//	IPostEffectState::SetEffectNo(kOutlineBlue);
	//}
	//if (sceneTime >= 360) {
	//	sceneTime = 0;
	//}
	if (input->TriggerKey(DIK_SPACE)) {
		fade->StartFadeIn();
	}
	if (fade->IsFadeOutComplete()) {
		sceneNo = 1;
	}

	TenQOBJ->worldTransform_.rotation_.x += 0.001f;
	TenQOBJ->worldTransform_.translation_.x = Lerp(TenQOBJ->worldTransform_.translation_.x, camera->transform_.translate.x, 0.005f);
	TenQOBJ->worldTransform_.translation_.z = Lerp(TenQOBJ->worldTransform_.translation_.z, camera->transform_.translate.z + 300, 0.05f);

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

		camera->Update();
		camera->Move();

		for (std::vector<Object3d *>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
			(*itr)->Update();

		}

		GridOBJ->Update();
		ConeOBJ->Update();
		TenQOBJ->Update();
		PlayerOBJ->Update();
		SkyBoxOBJ->Update();

		object3d_[0]->worldTransform_.rotation_.y += 0.001f;
		object3d_[1]->worldTransform_.rotation_.x += 0.01f;
		object3d_[1]->worldTransform_.rotation_.y += 0.01f;
		object3d_[1]->worldTransform_.rotation_.z += 0.01f;
		object3d_[2]->worldTransform_.rotation_.x += 0.01f;
		object3d_[2]->worldTransform_.rotation_.y -= 0.01f;
		object3d_[2]->worldTransform_.rotation_.z -= 0.01f;
		if (effectFlag == true) {
			sceneTime++;
		}

		///////////////Debug///////////////

		camera->CameraDebug();
	
		object3d_[0]->ModelDebug("JSONmodel0");
		object3d_[1]->ModelDebug("JSONmodel1");
		object3d_[2]->ModelDebug("JSONmodel2");
		object3d_[3]->ModelDebug("JSONmodel3");
		object3d_[4]->ModelDebug("JSONmodel4");

		GridOBJ->ModelDebug("grid");												
		ConeOBJ->ModelDebug("cone");
		TenQOBJ->ModelDebug("TenQ");
		SkyBoxOBJ->ModelDebug("SKY");

		PlayerOBJ->ModelDebug("chara");

		particle->Particledebug("uv", worldTransform);
		particle2->Particledebug("white", worldTransform2);

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
	GridOBJ->Draw(GRIDtextureHandle, camera);
	ConeOBJ->Draw(CONEtextureHandle, camera);
	TenQOBJ->Draw(TENQtextureHandle, camera);
	PlayerOBJ->Draw(UVtextureHandle, camera);
	SkyBoxOBJ->Draw(SKYtextureHandle, camera);

	particle->Draw(ParticleEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, UVtextureHandle, camera, demoRandPro, false);
	particle2->Draw(ParticleEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, WHITEtextureHandle, camera, demoRandPro, false);
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

