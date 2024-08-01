#include "GameManager.h"
#include <Windows.h>
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "SRVManager.h"
#include "PSOModel.h"
#include "PSOAnimationModel.h"
#include "PSOSprite.h"
#include "PSOParticle.h"
#include "PSOSkybox.h"
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
	pSOPostEffect->Init();

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
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.7f, 0.0f, 0.2f, 0.8f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.3f, 0.0f, 0.1f, 0.5f));
		ImGui::Begin("PostEffect", nullptr, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("PostEffect")) {
				if (ImGui::MenuItem("Normal")) {
					IPostEffectState::SetEffectNo(kFullScreen);
					currentStatus = "Normal effect applied";
				}
				/*if (ImGui::MenuItem("Outline")) {
					pSOPostEffect->CreatePipelineStateObjectLuminancebasedoutline();
					currentStatus = "Outline effect applied";
				}
				if (ImGui::MenuItem("Grayscale")) {
					pSOPostEffect->CreatePipelineStateObjectGrayscale();
					currentStatus = "Grayscale effect applied";
				}
				if (ImGui::MenuItem("Fog")) {
				    pSOPostEffect->CreatePipelineStateObjectFog();
					currentStatus = "Fog effect applied";
				}
				if (ImGui::MenuItem("Dissololve")) {
					pSOPostEffect->CreatePipelineStateObjectDissololve();
					currentStatus = "Dissolve effect applied";
				}
				if (ImGui::MenuItem("Gaussian")) {
					pSOPostEffect->CreatePipelineStateObjectGaussian();
					currentStatus = "Gaussian blur effect applied";
				}
				if (ImGui::MenuItem("Vignett")) {
					pSOPostEffect->CreatePipelineStateObjectVignett();
					currentStatus = "Vignette effect applied";
				}
				if (ImGui::MenuItem("Random")) {
					pSOPostEffect->CreatePipelineStateObjectRandom();
					currentStatus = "Random effect applied";
				}
				if (ImGui::MenuItem("Radialblur")) {
					pSOPostEffect->CreatePipelineStateObjectRadialblur();
					currentStatus = "Radial blur effect applied";
				}
				if (ImGui::MenuItem("Outline Black")) {
					pSOPostEffect->CreatePipelineStateObjectTest();
					currentStatus = "Outline Black effect applied";
				}
				if (ImGui::MenuItem("TV")) {
					pSOPostEffect->CreatePipelineStateObjectTest2();
					currentStatus = "TV effect applied";
				}
				if (ImGui::MenuItem("Outline Blue")) {
					pSOPostEffect->CreatePipelineStateObjectTest3();
					currentStatus = "Outline Blue effect applied";
				}
				if (ImGui::MenuItem("Outline Test")) {
					pSOPostEffect->CreatePipelineStateObjectTest4();
					currentStatus = "Outline Test effect applied";
				}
				if (ImGui::MenuItem("Test5")) {
					pSOPostEffect->CreatePipelineStateObjectTest5();
					currentStatus = "Test5 effect applied";
				}
				if (ImGui::MenuItem("chromatic aberration")) {
					pSOPostEffect->CreatePipelineStateObjectTest6();
					currentStatus = "chromatic aberration effect applied";
				}
				if (ImGui::MenuItem("HSV")) {
					pSOPostEffect->CreatePipelineStateObjectTest7();
					currentStatus = "HSV effect applied";
				}
				if (ImGui::MenuItem("Test8")) {
					pSOPostEffect->CreatePipelineStateObjectTest8();
					currentStatus = "Test8 effect applied";
				}
				if (ImGui::MenuItem("Test9")) {
					pSOPostEffect->CreatePipelineStateObjectTest9();
					currentStatus = "Test9 effect applied";
				}
				if (ImGui::MenuItem("Test10")) {
					pSOPostEffect->CreatePipelineStateObjectTest10();
					currentStatus = "Test10 effect applied";
				}*/

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Text("Status: %s", currentStatus);
		ImGui::Text("Scene : %d", currentSceneNo_);
		ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		

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