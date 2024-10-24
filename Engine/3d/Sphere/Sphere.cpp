#include "Sphere.h"
#include "WinAPI.h"
#include "DirectXCommon.h"
#include "mathFunction.h"
#include "Camera.h"
#include "SRVManager.h"
/**
* @file Sphere.cpp
* @brief 球体モデルを描画するクラス
*/
Sphere::Sphere() {

}

Sphere::~Sphere() {

}

void Sphere::Initialize(Camera* camera) {
	sWinAPI = WinAPI::GetInstance();
	sDirectXCommon_ = DirectXCommon::GetInstance();
	camera_ = camera;




	//バッファリソース

	// 実際に頂点リソースを作る
	vertexResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(VertexData) * 1536);

	vertexBufferView = CreateBufferView();


	// 頂点リソースにデータを書き込む
	vertexData_ = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	const uint32_t kSubdivision = 16;
	float pi = 3.1415f;
	const float kLonEvery = pi * 2.0f / (float)kSubdivision;  // 軽度分割1つ分の角度
	const float kLatEvery = pi / (float)kSubdivision;  // 緯度分割1つ分の角度


	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;// 現在の緯度
		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;// 現在の緯度

			const float latD = pi / kSubdivision;
			const float lonD = 2 * pi / kSubdivision;
			//左下
			vertexData_[start].position =
			{
						(cos(lat) * cos(lon)),
						(sin(lat)) ,
						(cos(lat) * sin(lon)),
			1.0f };
			vertexData_[start].normal = {
				vertexData_[start].position.x,
				vertexData_[start].position.y,
				vertexData_[start].position.z,
			};
			vertexData_[start].texcorrd = {
				float(lonIndex) / float(kSubdivision),
				1.0f - float(latIndex) / float(kSubdivision)

			};

			vertexData_[start + 1].position = {
				(cos(lat + latD) * cos(lon)) ,
				(sin(lat + latD)) ,
				(cos(lat + latD) * sin(lon)),
			1.0f };
			vertexData_[start + 1].normal = {
				vertexData_[start + 1].position.x,
				vertexData_[start + 1].position.y,
				vertexData_[start + 1].position.z,
			};
			vertexData_[start + 1].texcorrd = {
				float(lonIndex) / float(kSubdivision),
				1.0f - float(latIndex + 1) / float(kSubdivision)

			};

			vertexData_[start + 2].position = {
				(cos(lat) * cos(lon + lonD)),
				(sin(lat)) ,
				(cos(lat) * sin(lon + lonD)) ,
			1.0f };

			vertexData_[start + 2].normal = {
				vertexData_[start + 2].position.x,
				vertexData_[start + 2].position.y,
				vertexData_[start + 2].position.z,
			};
			vertexData_[start + 2].texcorrd = {
				float(lonIndex + 1) / float(kSubdivision),
				1.0f - float(latIndex) / float(kSubdivision)

			};

			vertexData_[start + 3].position = {
				(cos(lat + latD) * cos(lon)) ,
				(sin(lat + latD)) ,
				(cos(lat + latD) * sin(lon)),
			1.0f };

			vertexData_[start + 3].normal = {
				vertexData_[start + 3].position.x,
				vertexData_[start + 3].position.y,
				vertexData_[start + 3].position.z,
			};
			vertexData_[start + 3].texcorrd = {
				float(lonIndex) / float(kSubdivision),
				1.0f - float(latIndex + 1) / float(kSubdivision)

			};

			vertexData_[start + 4].position = {
				(cos(lat + latD) * cos(lon + lonD)),
				(sin(lat + latD)) ,
				(cos(lat + latD) * sin(lon + lonD)) ,
			1.0f };

			vertexData_[start + 4].normal = {
				vertexData_[start + 4].position.x,
				vertexData_[start + 4].position.y,
				vertexData_[start + 4].position.z,
			};
			vertexData_[start + 4].texcorrd = {
				float(lonIndex + 1) / float(kSubdivision),
				1.0f - float(latIndex + 1) / float(kSubdivision)

			};

			vertexData_[start + 5].position = {
				(cos(lat) * cos(lon + lonD)),
				(sin(lat)) ,
				(cos(lat) * sin(lon + lonD)) ,
			1.0f };

			vertexData_[start + 5].normal = {
				vertexData_[start + 5].position.x,
				vertexData_[start + 5].position.y,
				vertexData_[start + 5].position.z,
			};
			vertexData_[start + 5].texcorrd = {
				float(lonIndex + 1) / float(kSubdivision),
				1.0f - float(latIndex) / float(kSubdivision)

			};



		}
	}


	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(Material));

	materialBufferView = CreateBufferView();;
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 色のデータを変数から読み込み
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->enableLighting = true;
	materialData->uvTransform = MakeIdentity4x4();

	transformUv = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(transformUv.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(transformUv.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(transformUv.translate));
	materialData->uvTransform = uvTransformMatrix;

	\

		//バッファリソース
		// データを書き込む
		wvpData = nullptr;
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice().Get(), sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでいく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();

	directionalLightData = nullptr;
	directionalLightResource = Mesh::CreateBufferResource(sDirectXCommon_->GetDevice(), sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;


	////左下
	//vertexData[0] = { -0.5f,-0.5f,0.0f,1.0f };
	////上
	//vertexData[1] = { 0.0f,0.5f,0.0f,1.0f };
	////右下
	//vertexData[2] = { 0.5f,-0.5f,0.0f,1.0f };


	//マテリアルにデータを書き込む


	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = (float)sWinAPI->GetKClientWidth();
	viewport.Height = (float)sWinAPI->GetKClientHeight();
	viewport.TopLeftX = 1;
	viewport.TopLeftY = 1;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;


	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = sWinAPI->GetKClientWidth();
	scissorRect.top = 0;
	scissorRect.bottom = sWinAPI->GetKClientHeight();


}

void Sphere::Update() {

}

void Sphere::Draw(Transform transform, uint32_t texture) {
	pso_ = PSO::GetInstance();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(transformUv.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(transformUv.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(transformUv.translate));
	materialData->uvTransform = uvTransformMatrix;
	//// 色のデータを変数から読み込み
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, camera_->GetViewprojectionMatrix());
	wvpData->WVP = worldViewProjectionMatrix;
	sDirectXCommon_->GetCommandList()->RSSetViewports(1, &viewport);  //viewportを設定
	sDirectXCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect);    //Scirssorを設定:
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	sDirectXCommon_->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	sDirectXCommon_->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	sDirectXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);    //VBVを設定
	//sDirectXCommon_->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);    
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	sDirectXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	sDirectXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	sDirectXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	sDirectXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetGPUDescriptorHandle(texture));
	sDirectXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());
	// 描画！（DrawCall/ドローコール）・3頂点で1つのインスタンス。インスタンスについては今後
	sDirectXCommon_->GetCommandList()->DrawInstanced(1536, 1, 0, 0);
	//
}


D3D12_VERTEX_BUFFER_VIEW  Sphere::CreateBufferView() {
	D3D12_VERTEX_BUFFER_VIEW view{};

	//リソースの先頭のアドレスから使う
	view.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	view.SizeInBytes = sizeof(VertexData) * 1536;
	// 1頂点あたりのサイズ
	view.StrideInBytes = sizeof(VertexData);


	return view;
};

void Sphere::DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportmatrix, int color) {
	const uint32_t kSubdivision = 16;
	float pi = 3.1415f;
	const float kLonEvery = pi * 2.0f / (float)kSubdivision;  // 軽度分割1つ分の角度
	const float kLatEvery = pi / (float)kSubdivision;  // 緯度分割1つ分の角度


	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -pi / 2.0f + kLatEvery * latIndex;// 現在の緯度
		// 緯度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;// 現在の緯度

			const float latD = pi / kSubdivision;
			const float lonD = 2 * pi / kSubdivision;
			Vector3 a = {
				(cos(lat) * cos(lon)),
				(sin(lat)) ,
				(cos(lat) * sin(lon)) };

			Vector3 b = {
				(cos(lat + latD) * cos(lon)) ,
				(sin(lat + latD)) ,
				(cos(lat + latD) * sin(lon)) };

			Vector3 c = {
				(cos(lat) * cos(lon + lonD)),
				(sin(lat)) ,
				(cos(lat) * sin(lon + lonD)) };

			Vector3 d = {
				(cos(lat + latD) * cos(lon + lonD)),
				(sin(lat + latD)) ,
				(cos(lat + latD) * sin(lon + lonD)) };



		}
	}
}