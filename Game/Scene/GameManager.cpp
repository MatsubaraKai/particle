#include "GameManager.h"
#include <Windows.h>
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "SRVManager.h"
#include "PSOModel.h"
#include "PSOSprite.h"
#include "PSOParticle.h"
#include "PSOPostEffect.h"
#include "PostProcess.h"
#include "Input.h"
#include "Mesh.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#include "Audio.h"
#include "Camera.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Object3dCommon.h"
#include "Triangle.h"

#include "VertexData.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "mathFunction.h"

// コンストラクタ
GameManager::GameManager() {
	// 各シーンの排列
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<GameScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	sceneArr_[DEMO] = std::make_unique<DemoScene>();

	// 初期シーンの設定
	//sceneNo_ = TITLE; //GameManagerのクラスにISceneを継承させて触れるようにしているため正しいかは怪しい
	//input_ = Input::GetInstance();
}

GameManager::~GameManager() {}

const char kWindowTitle[] = "LE3A";

int GameManager::Run() {
	//DirectXCommon::D3DResourceLeakChecker leakCheck;

	WinAPI* sWinAPI = WinAPI::GetInstance();
	sWinAPI->Initialize(L"Game");

	DirectXCommon* sDirctX = DirectXCommon::GetInstance();
	sDirctX->Initialize();

	ImGuiCommon* imGuiCommon = ImGuiCommon::GetInstance();
	imGuiCommon->Initialize();

	Audio* sAudio = Audio::GetInstance();
	sAudio->Initialize();

	Object3dCommon* sObjectCommon = Object3dCommon::GetInstance();
	sObjectCommon->Init();

	ModelManager* sModelManager = ModelManager::GetInstance();
	sModelManager->init();

	TextureManager* sTextureManager = TextureManager::GetInstance();
	sTextureManager->Init();

	PSO* pso = PSO::GatInstance();
	pso->CreatePipelineStateObject();

	PSOSprite* psoSprite = PSOSprite::GatInstance();
	psoSprite->CreatePipelineStateObject();

	PSOParticle* psoParticle = PSOParticle::GatInstance();
	psoParticle->CreatePipelineStateObject();

	PSOPostEffect* pSOPostEffect = PSOPostEffect::GatInstance();
	pSOPostEffect->CreatePipelineStateObjectTest3();


	sceneArr_[currentSceneNo_]->Init();

	Input* sInput = Input::GetInstance();
	sInput->Initialize();
	// ウィンドウの×ボタンが押されるまでループ
	while (true)  // ゲームループ
	{
		// Windowsのメッセージ処理
		if (sWinAPI->ProcessMessage()) {
			// ゲームループを抜ける
			break;
		}
		// ゲームの処理の開始
		sDirctX->tempRender();
		// ImGui
		ImGuiCommon::GetInstance()->UICreate();
		//ImGuiの更新
		ImGuiCommon::GetInstance()->Update();

		sInput->Update();

		// 追跡するための変数

		ImGui::Begin("PostEffect");
		ImGui::Text("Status: %s", currentStatus);
		ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
		if (ImGui::Button("Normal    ")) {
			pSOPostEffect->CreatePipelineStateObject();
			currentStatus = "Normal effect applied";
		}
		if (ImGui::Button("Outline   ")) {
			pSOPostEffect->CreatePipelineStateObjectLuminancebasedoutline();
			currentStatus = "Outline effect applied";
		}
		if (ImGui::Button("Grayscale ")) {
			pSOPostEffect->CreatePipelineStateObjectGrayscale();
			currentStatus = "Grayscale effect applied";
		}
		if (ImGui::Button("Fog       ")) {
			pSOPostEffect->CreatePipelineStateObjectFog();
			currentStatus = "Fog effect applied";
		}
		if (ImGui::Button("Dissololve")) {
			pSOPostEffect->CreatePipelineStateObjectDissololve();
			currentStatus = "Dissolve effect applied";
		}
		if (ImGui::Button("Gaussian  ")) {
			pSOPostEffect->CreatePipelineStateObjectGaussian();
			currentStatus = "Gaussian blur effect applied";
		}
		if (ImGui::Button("Vignett   ")) {
			pSOPostEffect->CreatePipelineStateObjectVignett();
			currentStatus = "Vignette effect applied";
		}
		if (ImGui::Button("Random    ")) {
			pSOPostEffect->CreatePipelineStateObjectRandom();
			currentStatus = "Random effect applied";
		}
		if (ImGui::Button("Radialblur")) {
			pSOPostEffect->CreatePipelineStateObjectRadialblur();
			currentStatus = "Radial blur effect applied";
		}/*
		if (ImGui::Button("Depthbasedoutline")) {
			pSOPostEffect->CreatePipelineStateObjectDepthbasedoutline();
			currentStatus = "Depth based outline effect applied";
		}*/
		/*if (ImGui::Button("Boxfilter")) {
			pSOPostEffect->CreatePipelineStateObjectBoxfilter();
			currentStatus = "Box filter effect applied";
		}*/
		if (ImGui::Button("Outline Black")) {
			pSOPostEffect->CreatePipelineStateObjectTest();
			currentStatus = "Test effect applied";
		}
		if (ImGui::Button("Outline Blue")) {
			pSOPostEffect->CreatePipelineStateObjectTest3();
			currentStatus = "Test3 effect applied";
		}
		if (ImGui::Button("Outline Test")) {
			pSOPostEffect->CreatePipelineStateObjectTest4();
			currentStatus = "Test4 effect applied";
		}
		if (ImGui::Button("TV")) {
			pSOPostEffect->CreatePipelineStateObjectTest2();
			currentStatus = "Test2 effect applied";
		}
		if (ImGui::Button("Test5")) {
			pSOPostEffect->CreatePipelineStateObjectTest5();
			currentStatus = "Test5 effect applied";
		}
		if (ImGui::Button("chromatic aberration")) {
			pSOPostEffect->CreatePipelineStateObjectTest6();
			currentStatus = "chromatic aberration effect applied";
		}
		if (ImGui::Button("HSV")) {
			pSOPostEffect->CreatePipelineStateObjectTest7();
			currentStatus = "HSV effect applied";
		}
		if (ImGui::Button("Test8")) {
			pSOPostEffect->CreatePipelineStateObjectTest8();
			currentStatus = "Test8 effect applied";
		}
		if (ImGui::Button("Test9")) {
			pSOPostEffect->CreatePipelineStateObjectTest9();
			currentStatus = "Test9 effect applied";
		}
		if (ImGui::Button("Test10")) {
			pSOPostEffect->CreatePipelineStateObjectTest10();
			currentStatus = "Test10 effect applied";
		}
		ImGui::End();

		

		// シーンのチェック
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		// シーン変更チェック
		if (prevSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Init();
		}

		///
		/// ↓更新処理ここから
		///
		sceneArr_[currentSceneNo_]->Update(); // シーンごとの更新処理

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		sceneArr_[currentSceneNo_]->Draw();


		///
		/// ↑描画処理ここまで
		///
		sDirctX->BeginFrame();
		sDirctX->ChangeDepthStatetoRead();
		sceneArr_[currentSceneNo_]->PostDraw();
		// フレームの終了
		//スワップチェーン
		sDirctX->ChangeDepthStatetoRender();
		sDirctX->ViewChange();
		sAudio->GetIXAudio().Reset();
		// ESCキーが押されたらループを抜ける
		if (sceneArr_[currentSceneNo_]->GameClose()) {
			sceneArr_[currentSceneNo_]->Release();
			break;
		}
	}

	//出力ウィンドウへの文字出力
	OutputDebugStringA("Hello,DirectX!\n");


	/*------------------------------------------------------------

	-------------------------------------------------------------*/

	sModelManager->Finalize();
	sWinAPI->Finalize();
	//delete sWinAPI;
	sDirctX->Release();

	return 0;
}