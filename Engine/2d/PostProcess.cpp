#include "PostProcess.h"
#include "PSOPostEffect.h"
#include "Mesh.h"
#include <SRVManager.h>
/**
* @file PostProcess.cpp
* @brief PostProcess
*/
PostProcess::PostProcess()
{
}
PostProcess::~PostProcess() {
}
void PostProcess::Init()
{
	noiseTexture_ = TextureManager::StoreTexture("Resources/noise0.png");

}

void PostProcess::Update()
{
}

void PostProcess::Draw() {
	//materialData->projectionInverse = Inverse(camera_->GetProjectionMatrix());
	PSOPostEffect* pso_ = PSOPostEffect::GetInstance();
	pso_->EffectChangeCheck();
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	sDirectXCommon->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	sDirectXCommon->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定

	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	//sDirectXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pso_->SetCommand(this);
	// 描画（DrawCall/ドローコール）
	//sDirectXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	sDirectXCommon->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}

void PostProcess::Release()
{
}
