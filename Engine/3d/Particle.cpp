#include "Particle.h"
#include "Sprite.h"
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "Camera.h"
#include "mathFunction.h"
#include "Mesh.h"
#include "ImGuiCommon.h"

#include <numbers>



Particle::Particle() {};
void Particle::Initialize(Emitter emitter) {
	sWinAPI = WinAPI::GetInstance();
	sDirectXCommon = DirectXCommon::GetInstance();

	

	//rootParamerters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescripterTableを使う
	//rootParamerters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	//rootParamerters[0].DescriptorTable.pDescriptorRanges = descriptorRange_; // Tableの中身の配列を指定
	//rootParamerters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange_); // Tableで利用する数

	// Sprite用の頂点リソースを作る
	vertexResourceSprite_ = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(VertexData) * 4);

	// リソースを先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 4;
	// 1頂点あたりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);

	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));

	// 1枚目の三角形
	vertexDataSprite_[0].position = { -1.0f,-1.0f,0.0f,1.0f };//左下
	vertexDataSprite_[0].texcorrd = { 0.0f,1.0f };
	vertexDataSprite_[1].position = { -1.0f,1.0f,0.0f,1.0f }; // 左上
	vertexDataSprite_[1].texcorrd = { 0.0f,0.0f };
	vertexDataSprite_[2].position = { 1.0f,-1.0f,0.0f,1.0f }; // 右下
	vertexDataSprite_[2].texcorrd = { 1.0f,1.0f };

	vertexDataSprite_[3].position = { 1.0f,1.0f,0.0f,1.0f }; // 右上
	vertexDataSprite_[3].texcorrd = { 1.0f,0.0f };

	//vertexDataSprite_[0].normal = {
	//	vertexDataSprite_[0].position.x,
	//	vertexDataSprite_[0].position.y,
	//	vertexDataSprite_[0].position.z };

	//vertexDataSprite_[1].normal = {
	//	vertexDataSprite_[1].position.x,
	//	vertexDataSprite_[1].position.y,
	//	vertexDataSprite_[1].position.z };

	//vertexDataSprite_[2].normal = {
	//	vertexDataSprite_[2].position.x,
	//	vertexDataSprite_[2].position.y,
	//	vertexDataSprite_[2].position.z };

	//vertexDataSprite_[3].normal = {
	//	vertexDataSprite_[3].position.x,
	//	vertexDataSprite_[3].position.y,
	//	vertexDataSprite_[3].position.z };

	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(Material));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 色のデータを変数から読み込み
	materialData->color = {1.0f,1.0f,1.0f,1.0f};
	materialData->uvTransform = MakeIdentity4x4();
	materialData->enableLighting = false;

	// Transform変数の初期化
	
	// Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixResouceSprite = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(ParticleForGPU) * kNumMaxInstance);
	// 書き込むためのアドレスを取得
	transformationMatrixResouceSprite->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	// 単位行列を書き込んでおく
	
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
		instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	indexResourceSprite = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(uint32_t) * 6);
	// リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuint_tとする
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	// インデックスリソースにデータを書き込む
	uint32_t* indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0;
	indexDataSprite[1] = 1;
	indexDataSprite[2] = 2;
	indexDataSprite[3] = 1;
	indexDataSprite[4] = 3;
	indexDataSprite[5] = 2;

	transformUv = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};
	
	//std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	//for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
	//	//particles_[index] = MakeNewParticle(randomEngine);
	//	
	//}

	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	//// SRVを作成するDescriptorHeapの場所を決める
	//instancingSrvHandleCPU = sDirectXCommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	//instancingSrvHandleGPU = sDirectXCommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	//// 先頭はImGuiが使っているのでその次を使う
	//instancingSrvHandleCPU.ptr += sDirectXCommon->GetDevice()->(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * TextureManager::index_;
	//instancingSrvHandleGPU.ptr += sDirectXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * TextureManager::index_;
	// 
	instancingSrvHandleCPU = sDirectXCommon->GetCPUDescriptorHandle(sDirectXCommon->GetSrvDescriptorHeap().Get(), sDirectXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), TextureManager::index_);
	instancingSrvHandleGPU = sDirectXCommon->GetGPUDescriptorHandle(sDirectXCommon->GetSrvDescriptorHeap().Get(), sDirectXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),TextureManager::index_);
	// SRVの生成
	sDirectXCommon->GetDevice()->CreateShaderResourceView(transformationMatrixResouceSprite.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
	TextureManager::PlusIndex();
	directionalLightData = nullptr;
	directionalLightResource = Mesh::CreateBufferResource(sDirectXCommon->GetDevice(), sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;
	emitter_ = emitter;
	emitter_.count =6;
	emitter_.frequency = 0.02f;// 0.5秒ごとに発生
	emitter_.frequencyTime = 0.0f;// 発生頻度用の時刻、0で初期化
};
//void Sprite::Update() {
//
//};

void Particle::Draw(Emitter emitter,const Vector3& worldTransform, uint32_t texture,  Camera* camera, const RandRangePro& randRange,bool scaleAddFlag) {
	pso_ = PSOParticle::GatInstance();
	
	emitter_.count = emitter.count;
	emitter_.transform = { emitter.transform.scale,{0.0f,0.0f,0.0f},worldTransform };
	randRange_ = randRange;

	randRange_ = 
	{
	{0.3f,0.7f},
	{0.2f,0.5f},
	{-0.5f,0.3f}
	};
	//materialData->color = {1.0f,1.0f,1.0f,1.0f};
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix,camera->cameraMatrix_);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	

	emitter_.frequencyTime += kDeltaTime;// 時刻を進める
	if (emitter_.frequency <= emitter_.frequencyTime) {// 頻度より大きいなら発生
		particles_.splice(particles_.end(), particle->Emission(emitter_, randomEngine, worldTransform , randRange));
		emitter_.frequencyTime -= emitter_.frequency;// 余計に過ぎた時間も加味して頻度計算する

	}

	uint32_t numInstance = 0;// 描画すべきインスタンス数
	// Sprite用のWorldViewProjectMatrixを作る
	for (std::list<ParticlePro>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime){
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
	
		(*particleIterator).transform.translate.x += (*particleIterator).velocity.x * kDeltaTime;
		(*particleIterator).transform.translate.y += (*particleIterator).velocity.y * kDeltaTime;
		(*particleIterator).transform.translate.z += (*particleIterator).velocity.z * kDeltaTime;
		if (scaleAddFlag) {
			(*particleIterator).transform.scale = Add((*particleIterator).transform.scale, { 0.1f ,0.1f,0.1f });
		}
		(*particleIterator).currentTime += kDeltaTime;
		// (*particleIterator).color = { 1.0f,1.0f,1.0f,1.0f };
		float alpha = 0.5f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
		//transforms_[index].rotate.x += 0.1f;
		Matrix4x4 worldMatrix = Multiply(MakeScaleMatrix((*particleIterator).transform.scale), Multiply(billboardMatrix, MakeTranslateMatrix((*particleIterator).transform.translate)));
		//Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->viewMatrix_, camera->projectionMatrix_));
		if (numInstance < kNumMaxInstance) {
			instancingData[numInstance].WVP = worldViewProjectionMatrix;
			instancingData[numInstance].World = worldMatrix;
			instancingData[numInstance].color = (*particleIterator).color;
			instancingData[numInstance].color.w = alpha;
		}
		++numInstance; // 生きているparticluの数を1使うんとする
		++particleIterator;

	}
	textureManager_ = TextureManager::GetInstance();
	sDirectXCommon->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	sDirectXCommon->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	sDirectXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_); // VBVを設定
	sDirectXCommon->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sDirectXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// TransformationmatrixCBufferの場所を設定
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1,instancingSrvHandleGPU);
	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->textureSrvHandleGPU_[texture]);
	sDirectXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	// 描画（DrawCall/ドローコール）
	//sDirectXCommon->GetCommandList()->DrawInstanced(6, 1, 0, 0);
	sDirectXCommon->GetCommandList()->DrawIndexedInstanced(6, numInstance, 0, 0, 0);
}




Particle::ParticlePro Particle::MakeNewParticle(std::mt19937& randomEngine, const Vector3& scale, const Vector3& translate, const RandRangePro& randRange)
{
	RandRangePro ran = randRange;
	std::uniform_real_distribution<float> distribution(-0.8f, 0.8f);
	std::uniform_real_distribution<float> distriposX(ran.rangeX.x, ran.rangeX.y);//distriposX(-0.7f, -0.3
	std::uniform_real_distribution<float> distriposY(ran.rangeY.x,ran.rangeY.y );//  distriposY(0.2f, 0.5f)
	std::uniform_real_distribution<float> distriposZ(ran.rangeZ.x, ran.rangeZ.y);// distriposZ(-0.5f, 0.3f
	//std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(0.0f, 1.5f);

	Particle::ParticlePro particle;
	particle.transform.scale = scale;
	particle.transform.rotate = { 0.0f,0.0f,0.0f };

	// 位置と速度を[-1,1]でランダムに初期化
	Vector3 randomTranslate = { distriposX(randomEngine), distriposY(randomEngine), distriposZ(randomEngine) };
	particle.transform.translate = Add(translate,randomTranslate);
	particle.velocity = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.color = { 1.0f,1.0f ,1.0f,0.7f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}

std::list<Particle::ParticlePro> Particle::Emission(const Emitter& emitter, std::mt19937& randEngine, const Vector3& worldTransform, const RandRangePro& randRange)
{
	std::list<Particle::ParticlePro> particles;
	for (uint32_t count = 0; count < emitter.count; ++count) {
		particles.push_back(MakeNewParticle(randEngine, emitter.transform.scale,emitter.transform.translate,randRange));

	}
	return particles;
}

D3D12_VERTEX_BUFFER_VIEW Particle::CreateBufferView() {
	D3D12_VERTEX_BUFFER_VIEW view{};

	//リソースの先頭のアドレスから使う
	view.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	view.SizeInBytes = sizeof(VertexData) * 6;
	// 1頂点あたりのサイズ
	view.StrideInBytes = sizeof(VertexData);


	return view;
};
