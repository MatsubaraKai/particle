#include "ClearScene.h"
#include "ImGuiCommon.h"
void ClearScene::Init()
{
	
	int Width = 0;
	int Height = 0;

	input_ = Input::GetInstance();
	camera_ = new Camera;
	camera_->Initialize();
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/uvChecker.png");//画像読み込み
	texture2_ = TextureManager::GetInstance()->StoreTexture("Resources/GLAY.png");//画像読み込み
	soundData = Audio::SoundLoadWave("Resources/fanfare.wav");
	PushTexture_ = TextureManager::GetInstance()->StoreTexture("Resources/BLUE.png");
	Stage1model_ = new Model();
	Stage1model_->Initialize("Resources/Car", "car.obj", color);
	//Pushmodel_->Initialize("Resources/")
	SlectSprite_ = new Sprite;
	SlectSprite_->Initialize(color);

	SelectNumber = 0;
	PushSprite_ = new Sprite;
	PushSprite_->Initialize(color);

	PushTransform_.Initialize();
	model_ = new Model();
	model_->Initialize("Resources/box", "box.obj", color);

	rearLeft = {
		{-0.9f,-0.7f},
		{0.1f,0.4f},
		{-0.5f,0.3f}
	};

	rearRight = {
		{-4.0f,10.0f},
		{-7.0f,10.0f},
		{-0.5f,0.3f}
	};

	smokeEmitter_.count = 6;
	smokeEmitter_.frequency = 0.02f;
	smokeEmitter_.frequencyTime = 0.0f;
	smokeEmitter_.transform.scale = { 0.2f,0.2f,0.2f };

	lightEmitter_.count = 4;
	lightEmitter_.frequency = 0.2f;
	lightEmitter_.frequencyTime = 0.0f;
	lightEmitter_.transform.scale = { 0.5f,0.3f,0.3f };

	particle = new Particle();
	particle->Initialize(smokeEmitter_);
	particle2 = new Particle();
	particle2->Initialize(lightEmitter_);
	

	camera_->Initialize();
	worldTransform_.translation_.x = 10;
	worldTransform_.translation_.z = 40;
	modelWorldTransform_.translation_.x = 0;
	modelWorldTransform_.translation_.z = 30;
	modelWorldTransform_.UpdateMatrix();
}

void ClearScene::Update()
{
	XINPUT_STATE joyState{};
	ImGui::Begin("Demo");
	ImGui::DragFloat3("CameraTranslate", &cameraPos.x,0.1f);
	ImGui::Text("Push A D ModelMoveX");
	ImGui::Text("GamePad ThumbLY ModelMoveY");
	ImGui::Text("Trigger L Sound");
	
	ImGui::End();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	sceneTime++;

	if (Input::GetInstance()->PushKey(DIK_A)) {
		modelWorldTransform_.translation_.x -= 0.1f;
	}
	else if (Input::GetInstance()->PushKey(DIK_D)) {
		modelWorldTransform_.translation_.x += 0.1f;
	}
	
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		modelWorldTransform_.translation_.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * 0.04f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_L)) {
		Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), soundData, false);
	}
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
		sceneTime = 0;
	}
	modelWorldTransform_.UpdateMatrix();

	PushTransform_.UpdateMatrix();
	
	Vector2 size;
	size.x = 300.0f;
	size.y = 200.0f;

	Vector2 pos = { 0.0f,0.0f };
	SlectSprite_->SetSize(size);
	SlectSprite_->SetPosition(pos);
	SlectSprite_->Update();

	Vector2 size2;
	size2.x = 480.0f;
	size2.y = 250.0f;
	Vector2 pos2 = { 800.0f,0.0f };
	PushSprite_->SetSize(size2);
	PushSprite_->SetPosition(pos2);
	PushSprite_->Update();
	

	camera_->SetCameraPosition(cameraPos);
	camera_->Update();

}
void ClearScene::Draw()
{
	Stage1model_->Draw(worldTransform_, texture_, camera_, color);
	SlectSprite_->Draw(texture_, color);
	PushSprite_->Draw(PushTexture_,color);

	model_->Draw(modelWorldTransform_, texture2_, camera_, color);

	particle->Draw(lightEmitter_, { worldTransform_.translation_.x ,worldTransform_.translation_.y ,worldTransform_.translation_.z  }, texture_, camera_, rearLeft, true);
	particle2->Draw(smokeEmitter_, { -worldTransform_.translation_.x  ,worldTransform_.translation_.y ,worldTransform_.translation_.z }, texture2_, camera_, rearRight, true);

}

void ClearScene::Release() {

}


// ゲームを終了
int ClearScene::GameClose()
{
	return false;
}