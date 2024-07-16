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
	camera->SetTranslate(cameraPos);
	input = Input::GetInstance();

	textureHandle = TextureManager::StoreTexture("Resources/uvChecker.png");
	textureHandle2 = TextureManager::StoreTexture("Resources/white.png");
	textureHandle3 = TextureManager::StoreTexture("Resources/game/cone.png");
	fadeTex = TextureManager::StoreTexture("Resources/black.png");

	ModelManager::GetInstance()->LoadModel("Resources/human", "sneakWalk.gltf");
	ModelManager::GetInstance()->LoadAnimationModel("Resources/AnimatedCube", "AnimatedCube.gltf");
	ModelManager::GetInstance()->LoadModel("Resources/game", "grid.obj");
	ModelManager::GetInstance()->LoadModel("Resources/game", "cone.obj");

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

	worldTransform.Initialize();
	worldTransform2.Initialize();
	GridTransform.Initialize();

	worldTransform.translation_.x = 0;
	worldTransform2.translation_.x = 5;
	GridTransform.scale_.x = 20;
	GridTransform.scale_.z = 20;

	worldTransform.UpdateMatrix();
	worldTransform2.UpdateMatrix();
	GridTransform.UpdateMatrix();

	postProcess_ = new PostProcess();
	postProcess_->SetCamera(camera);
	postProcess_->Init();

	GridOBJ = new Object3d();
	GridOBJ->Init();
	object3d2 = new Object3d();
	object3d2->Init();
	WallOBJ = new Object3d();
	WallOBJ->Init();

	//object3d->SetModel("sneakWalk.gltf");
	//object3d->SetModel("ball.obj");
	GridOBJ->SetModel("grid.obj");
	GridOBJ->SetWorldTransform(GridTransform);
	WallOBJ->SetModel("cone.obj");
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
	WallOBJ->Update();
	object3d2->Update();


	sceneTime++;

	///////////////Debug///////////////

	camera->CameraDebug();

	object3d_[0]->ModelDebug("JSONmodel");
	object3d_[1]->ModelDebug("JSONmodel2");
	object3d_[2]->ModelDebug("JSONmodel3");
	object3d_[3]->ModelDebug("JSONmodel4");

	fadeSprite->SpriteDebug("fadesprite");

	GridOBJ->ModelDebug("grid");
	WallOBJ->ModelDebug("cone");
	object3d2->ModelDebug("chara");

	particle->Particledebug("uv",worldTransform);
	particle2->Particledebug("white",worldTransform2);

	ImGui::Begin("color");
	float color[4] = { material2.color.x,material2.color.y,material2.color.z,material2.color.w };
	ImGui::DragFloat4("color", color, 0.01f);
	material.color = { color[0],color[1],color[2],color[3] };
	ImGui::End();
	ImGui::Begin("Space:FadeIn");
	ImGui::Checkbox("FadeIn", &isFadingIn);
	ImGui::End();

}
void DemoScene::Draw()
{
	for (std::vector<Object3d*>::iterator itr = object3d_.begin(); itr != object3d_.end(); itr++) {
		(*itr)->Draw(textureHandle, camera);
	}
	//demoSprite->Draw(textureHandle,{1.0f,1.0f,1.0f,1.0f});
	GridOBJ->Draw(textureHandle2, camera);
	WallOBJ->Draw(textureHandle3, camera);
	object3d2->Draw(textureHandle, camera);
	particle->Draw(demoEmitter_, { worldTransform.translation_.x,worldTransform.translation_.y,worldTransform.translation_.z + 5 }, textureHandle, camera, demoRandPro, false);
	particle2->Draw(demoEmitter_, { worldTransform2.translation_.x,worldTransform2.translation_.y,worldTransform2.translation_.z + 5 }, textureHandle2, camera, demoRandPro, false);
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


