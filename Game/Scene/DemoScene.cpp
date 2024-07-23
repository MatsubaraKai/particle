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
	cameraPos.y = 3.0f;
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();

	UVtextureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	WHITEtextureHandle = TextureManager::StoreTexture("Resources/white.png");
	CONEtextureHandle = TextureManager::StoreTexture("Resources/game/cone.png");
	//TENQtextureHandle = TextureManager::StoreTexture("Resources/game/world.png");
	TENQtextureHandle = TextureManager::StoreTexture("Resources/game/world2.png");
	FADEtextureHandle = TextureManager::StoreTexture("Resources/black.png");
	GRIDtextureHandle = TextureManager::StoreTexture("Resources/cian.png");

	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadAnimationModel("Resources/AnimatedCube", "AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/game", "grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "cone.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "world.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "world2.obj");

	Loder::LoadJsonFile("Resources", "TL", object3d_, camera);

	fadeSprite = new Sprite();
	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });
	material2.color = { 1.0f,1.0f,1.0f,0.0f };
	material2.enableLighting = true;

	demoSprite = new Sprite();
	demoSprite->Init({ 0.0f,0.0f }, { 600.0f,600.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/uvChecker.png");
	material.color = { 1.0f,1.0f,1.0f,1.0f };
	material.enableLighting = false;

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	GridOBJ = new Object3d();
	GridOBJ->Init();
	object3d2 = new Object3d();
	object3d2->Init();
	ConeOBJ = new Object3d();
	ConeOBJ->Init();
	TenQOBJ = new Object3d();
	TenQOBJ->Init();

	worldTransform.Initialize();
	worldTransform2.Initialize();
	GridTransform.Initialize();
	TenQTransform.Initialize();

	worldTransform.translation_.x = 0;
	worldTransform2.translation_.x = 5;

	GridTransform.scale_.x = 20;
	GridTransform.scale_.z = 20;
	GridTransform.UpdateMatrix();
	GridOBJ->SetWorldTransform(GridTransform);
	//忘れそうなのでメモ ドーナツ型のOBJを作って回転させればループするマップができる
	TenQTransform.translation_.y = 370.0f;
	TenQTransform.translation_.z = 300.0f;
	TenQTransform.scale_.x = -2.0f;
	TenQTransform.scale_.y = 2.0f;
	TenQTransform.scale_.z = 2.0f;
	
	TenQOBJ->SetWorldTransform(TenQTransform);

	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	
	//object3d->SetModel("sneakWalk.gltf");
	//object3d->SetModel("ball.obj");
	GridOBJ->SetModel("grid.obj");
	ConeOBJ->SetModel("cone.obj");
	TenQOBJ->SetModel("world2.obj");
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
	StartFadeOut();
}

void DemoScene::Update()
{
	TenQOBJ->worldTransform_.rotation_.x += 0.001f;
	TenQOBJ->worldTransform_.translation_.x = Lerp(TenQOBJ->worldTransform_.translation_.x, camera->transform_.translate.x, 0.005f);
	TenQOBJ->worldTransform_.translation_.z = Lerp(TenQOBJ->worldTransform_.translation_.z, camera->transform_.translate.z + 300, 0.005f);
	//TenQOBJ->worldTransform_.translation_.z = camera->transform_.translate.z + 300;
	if (isFadeOut == true)
	{
		UpdateFadeOut();
	}
	if (input->TriggerKey(DIK_SPACE)) {
		StartFadeIn();
	}
	if (isFadingIn == true)
	{
		UpdateFadeIn();
	}
	
	camera->Update();
	camera->Move();
	fadeSprite->Update();

	demoSprite->Update();
	for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
		(*itr)->Update();
		
	}

	GridOBJ->Update();
	ConeOBJ->Update();
	TenQOBJ->Update();
	object3d2->Update();
	
	object3d_[0]->worldTransform_.rotation_.y += 0.001f;
	object3d_[1]->worldTransform_.rotation_.x += 0.01f;
	object3d_[1]->worldTransform_.rotation_.y += 0.01f;
	object3d_[1]->worldTransform_.rotation_.z += 0.01f;
	object3d_[2]->worldTransform_.rotation_.x += 0.01f;
	object3d_[2]->worldTransform_.rotation_.y -= 0.01f;
	object3d_[2]->worldTransform_.rotation_.z -= 0.01f;
	sceneTime++;
	///////////////Debug///////////////

	camera->CameraDebug();

	object3d_[0]->ModelDebug("JSONmodel");
	object3d_[1]->ModelDebug("JSONmodel2");
	object3d_[2]->ModelDebug("JSONmodel3");
	//object3d_[3]->ModelDebug("JSONmodel4");

	fadeSprite->SpriteDebug("fadesprite");

	GridOBJ->ModelDebug("grid");
	ConeOBJ->ModelDebug("cone");
	TenQOBJ->ModelDebug("TenQ");
	
	object3d2->ModelDebug("chara");

	particle->Particledebug("uv",worldTransform);
	particle2->Particledebug("white",worldTransform2);

	ImGui::Begin("color");
	float color[4] = { material2.color.x,material2.color.y,material2.color.z,material2.color.w };
	ImGui::DragFloat4("color", color, 0.01f);
	material2.color = { color[0],color[1],color[2],color[3] };
	ImGui::End();
	ImGui::Begin("Space:FadeIn");
	ImGui::Checkbox("FadeIn", &isFadingIn);
	ImGui::End();
}
void DemoScene::Draw()
{
	for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
		(*itr)->Draw(CONEtextureHandle, camera);
	}
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	GridOBJ->Draw(GRIDtextureHandle, camera);
	ConeOBJ->Draw(CONEtextureHandle, camera);
	TenQOBJ->Draw(TENQtextureHandle, camera);
	object3d2->Draw(UVtextureHandle, camera);
	particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, UVtextureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, WHITEtextureHandle, camera, demoRandPro, false);
	fadeSprite->Draw(FADEtextureHandle, material2.color);
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
		sceneNo = 1;
	}
}


