#include "PostProcess.h"
#include "PSOCopyImage.h"
#include "Mesh.h"
#include <SRVManager.h>
PostProcess::PostProcess()
{
}
void PostProcess::Init()
{
	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(PostMaterial));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->projectionInverse = camera_->GetViewprojectionMatrix();

	noiseTexture_ = TextureManager::StoreTexture("Resources/noise0.png");

}

void PostProcess::Update()
{
	
}

void PostProcess::Draw() {
	materialData->projectionInverse = Inverse(camera_->GetProjectionMatrix());
	PSOCopyImage* pso_ = PSOCopyImage::GatInstance();
	DirectXCommon* sDirectXCommon = DirectXCommon::GetInstance();
	sDirectXCommon->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	sDirectXCommon->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	//sDirectXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_); // VBVを設定
	//sDirectXCommon->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sDirectXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(0, SRVManager::GetInstance()->GetGPUDescriptorHandle(sDirectXCommon->GetRenderIndex()));

	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, SRVManager::GetInstance()->GetGPUDescriptorHandle(sDirectXCommon->GetDepthIndex()));

	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetInstance()->GetGPUDescriptorHandle(noiseTexture_));
	// マテリアルCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, materialResource->GetGPUVirtualAddress());
	// 描画（DrawCall/ドローコール）
	//sDirectXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	sDirectXCommon->GetCommandList()->DrawInstanced(3, 1, 0, 0);
}
