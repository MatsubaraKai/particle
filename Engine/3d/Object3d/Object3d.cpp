#include "Object3d.h"
#include "Modelmanager.h"
#include "Object3dCommon.h"
void Object3d::Init()
{

	WinAPI* sWinAPI = WinAPI::GetInstance();
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	worldTransform_.Initialize();

	//バッファリソース
	// データを書き込む
	wvpData = nullptr;
	// WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(TransformationMatrix));
	// 書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでいく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
	// カメラ用
	cameraForGPUData_ = nullptr;
	cameraForGPUResource_ = Mesh::CreateBufferResource(directXCommon->GetDevice(), sizeof(CameraForGPU));
	// 書き込むためのアドレスを取得
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));

	cameraForGPUData_->worldPosition = { 1.0f,1.0f,-5.0f };

}

void Object3d::Update()
{
	worldTransform_.UpdateMatrix();


}

void Object3d::Draw(uint32_t texture, Camera* camera)
{
	PSO* pso = PSO::GatInstance();
	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	cameraForGPUData_->worldPosition = camera->GetTransform().translate;
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldTransform_.matWorld_, camera->GetViewprojectionMatrix());

	//directionalLightData->direction =  Normalize(directionalLightData->direction);
	directXCommon->GetCommandList()->SetGraphicsRootSignature(pso->GetProperty().rootSignature.Get());
	directXCommon->GetCommandList()->SetPipelineState(pso->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraForGPUResource_->GetGPUVirtualAddress());
	// 3Dモデルが割り当てられていれば描画する
	if (model_) {
		wvpData->WVP = Multiply(model_->GetAniMatrix(), worldViewProjectionMatrix);
		wvpData->World = Multiply(model_->GetAniMatrix(), worldTransform_.matWorld_);
		model_->Draw(texture, { { 1.0f,1.0f,1.0f,1.0f },true
			}, { { 1.0f,1.0,1.0,1.0f } ,{ 0.0f,-1.0f,0.0f },0.5f });
	}

}

void Object3d::Release()
{

}

void Object3d::SetModel(const std::string& filePath)
{
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData; // 構築するMataData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ1行を格納するもの

	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.z *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;

			texcoord.y *= -1.0f;// -texcoord.y; //- texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.z *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');// 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);

				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				//position.x *= -1.0f;
				//texcoord.y = 1.0f - texcoord.y;
				//normal.x *= -1.0f;

				VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);

				triangle[faceVertex] = { position,texcoord,normal };

			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		}
		else if (identifier == "mtllib") {
			// materialtemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}

	}
	return modelData;
}

MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;// 構築するMaterialData
	std::string line; // ファイルから読んだ1行をかくのうするもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identiferに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}

	}

	return materialData;
}
