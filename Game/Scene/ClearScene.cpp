#include "ClearScene.h"
#include "ImGuiCommon.h"
void ClearScene::Init()
{
	int Width = 0;
	int Height = 0;

	input_ = Input::GetInstance();
	camera_ = new Camera;
	camera_->Initialize();
	texture_ = TextureManager::GetInstance()->StoreTexture("Resources/uvChecker.png");
	model2_ = new Model();
	model2_->Initialize("Resources/box", "box.obj", color);
	sprite_ = new Sprite;
	sprite_->Initialize(color);

	SelectNumber = 0;
	model_ = new Model();
	model_->Initialize("Resources/box", "box.obj", color);

	lightEmitter_.count = 16;
	lightEmitter_.frequency = 0.001f;
	lightEmitter_.frequencyTime = 0.0f;
	lightEmitter_.transform.scale = { 0.5f,0.5f,0.5f };

	particle = new Particle();
	particle->Initialize(lightEmitter_);

	camera_->Initialize();
	worldTransform_.translation_.z = 40;
	modelWorldTransform_.translation_.z = 30;
	modelWorldTransform2_.translation_.z = 30;
	modelWorldTransform2_.translation_.x = 5;
	modelWorldTransform_.UpdateMatrix();
	modelWorldTransform2_.UpdateMatrix();
}

void ClearScene::Update()
{
	XINPUT_STATE joyState{};
	ImGui::Begin("MOVE");
	ImGui::Text("MODEL1: WASD");
	ImGui::Text("MODEL2: ARROW KEY");
	ImGui::Text("SPACE to TITLE");
	ImGui::End();
	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	worldTransform_.UpdateMatrix();

	sceneTime++;

	if (Input::GetInstance()->PushKey(DIK_A)) {
		modelWorldTransform_.translation_.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		modelWorldTransform_.translation_.x += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_W)) {
		modelWorldTransform_.translation_.y += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		modelWorldTransform_.translation_.y -= 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_LEFTARROW)) {
		modelWorldTransform2_.translation_.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWNARROW)) {
		modelWorldTransform2_.translation_.y -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_UPARROW)) {
		modelWorldTransform2_.translation_.y += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHTARROW)) {
		modelWorldTransform2_.translation_.x += 0.1f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
		sceneTime = 0;
	}
	modelWorldTransform_.UpdateMatrix();
	modelWorldTransform2_.UpdateMatrix();
	
	Vector2 size = {600.0f, 400.0f};
	Vector2 pos = { 0.0f,0.0f };
	sprite_->SetSize(size);
	sprite_->SetPosition(pos);
	sprite_->Update();
	
	camera_->SetCameraPosition(cameraPos);
	camera_->Update();
}
void ClearScene::Draw()
{
	model2_->Draw(modelWorldTransform2_, texture_, camera_, color);
	model_->Draw(modelWorldTransform_, texture_, camera_, color);
	particle->Draw(lightEmitter_, { worldTransform_.translation_.x ,worldTransform_.translation_.y ,worldTransform_.translation_.z  }, texture_, camera_, rear, true);
	sprite_->Draw(texture_, color);
}

void ClearScene::Release() {

}


// ゲームを終了
int ClearScene::GameClose()
{
	return false;
}