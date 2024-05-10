#include "Model.h"
#include "SRVManager.h"


Model::Model() {}
Model::~Model()
{
}
;
ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filePath)
{
	Assimp::Importer importer;
	std::string filePathA = directoryPath + "/" + filePath;
	const aiScene* scene = importer.ReadFile(filePathA.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMaterials()); // メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応
		// ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);// 三角形のみサポート
			// ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x, normal.y ,normal.z };
				vertex.texcorrd = { texcoord.x,texcoord.y };
				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData_.vertices.push_back(vertex);
			}

		}
	}

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData_.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	modelData_.rootNode = ReadNode(scene->mRootNode);


	//ModelData modelData; // 構築するMataData
	//std::vector<Vector4> positions; // 位置
	//std::vector<Vector3> normals; // 法線
	//std::vector<Vector2> texcoords; // テクスチャ座標
	//std::string line; // ファイルから読んだ1行を格納するもの

	//std::ifstream file(directoryPath + "/" + filePath); // ファイルを開く
	//assert(file.is_open()); // とりあえず開けなかったら止める

	//while (std::getline(file, line)) {
	//	std::string identifier;
	//	std::istringstream s(line);
	//	s >> identifier; // 先頭の識別子を読む

	//	// identifierに応じた処理
	//	if (identifier == "v") {
	//		Vector4 position;
	//		s >> position.x >> position.y >> position.z;
	//		position.w = 1.0f;
	//		position.z *= -1.0f;
	//		positions.push_back(position);
	//	}
	//	else if (identifier == "vt") {
	//		Vector2 texcoord;
	//		s >> texcoord.x >> texcoord.y;

	//		texcoord.y *= -1.0f;// -texcoord.y; //- texcoord.y;
	//		texcoords.push_back(texcoord);
	//	}
	//	else if (identifier == "vn") {
	//		Vector3 normal;
	//		s >> normal.x >> normal.y >> normal.z;
	//		normal.z *= -1.0f;
	//		normals.push_back(normal);
	//	}
	//	else if (identifier == "f") {
	//		VertexData triangle[3];
	//		// 面は三角形限定。その他は未対応
	//		for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
	//			std::string vertexDefinition;
	//			s >> vertexDefinition;
	//			// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
	//			std::istringstream v(vertexDefinition);
	//			uint32_t elementIndices[3];
	//			for (int32_t element = 0; element < 3; ++element) {
	//				std::string index;
	//				std::getline(v, index, '/');// 区切りでインデックスを読んでいく
	//				elementIndices[element] = std::stoi(index);

	//			}
	//			// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
	//			Vector4 position = positions[elementIndices[0] - 1];
	//			Vector2 texcoord = texcoords[elementIndices[1] - 1];
	//			Vector3 normal = normals[elementIndices[2] - 1];
	//			//position.x *= -1.0f;
	//			//texcoord.y = 1.0f - texcoord.y;
	//			//normal.x *= -1.0f;

	//			VertexData vertex = { position, texcoord, normal };
	//			modelData.vertices.push_back(vertex);

	//			triangle[faceVertex] = { position,texcoord,normal };

	//		}
	//		modelData.vertices.push_back(triangle[2]);
	//		modelData.vertices.push_back(triangle[1]);
	//		modelData.vertices.push_back(triangle[0]);

	//	}
	//	else if (identifier == "mtllib") {
	//		// materialtemplateLibraryファイルの名前を取得する
	//		std::string materialFilename;
	//		s >> materialFilename;
	//		// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
	//		modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
	//	}

	//}
	return modelData_;

};

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
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
Node Model::ReadNode(aiNode* node)
{
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのloacalMatrixを習得
	aiLocalMatrix.Transpose(); // 列ベクトル形式をベクトル形式に転置

	//result.localMatrix.m[0][0] = aiLocalMatrix[0][0]; // 他の要素も同様に
	for (uint32_t index = 0; index < 4; ++index) {
		for (uint32_t j = 0; j < 4; ++j) {
			//// 再帰的に読んで階層構造を作っていく
			result.localMatrix.m[index][j] = aiLocalMatrix[index][j];
		}
	}
	// ...
	result.name = node->mName.C_Str(); // Node名を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}
;

void Model::Initialize(const std::string& directoryPath, const std::string& filename, const Material& material) {
	WinAPI* sWinAPI = WinAPI::GetInstance();
	directXCommon_ = DirectXCommon::GetInstance();

	// モデル読み込み
	modelData_ = LoadObjFile(directoryPath, filename);


	// 頂点リソースを作る
	vertexResource_ = Mesh::CreateBufferResource(directXCommon_->GetDevice(), sizeof(VertexData) * modelData_.vertices.size());
	// 頂点バッファビューを作成する
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size()); // 使用するリソースのサイズは頂点のサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	// 実際に頂点リソースを作る
	materialResource = Mesh::CreateBufferResource(directXCommon_->GetDevice(), sizeof(Material));

	/*materialBufferView = CreateBufferView();;*/
	// 頂点リソースにデータを書き込む
	materialData = nullptr;
	// 書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// 色のデータを変数から読み込み
	materialData->color = material.color;
	materialData->enableLighting = material.enableLighting;
	materialData->uvTransform = MakeIdentity4x4();
	materialData->shininess = material.shininess;

	transformUv = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};



	directionalLightData = nullptr;
	directionalLightResource = Mesh::CreateBufferResource(directXCommon_->GetDevice(), sizeof(DirectionalLight));
	// 書き込むためのアドレスを取得
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// デフォルト値はとりあえず以下のようにしておく
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData->intensity = 1.0f;


	//worldTransform_.Initialize();

};

void Model::Update() {
	//worldTransform_.UpdateMatrix();
};


void Model::Draw(uint32_t texture, const Material& material, const DirectionalLight& dire) {

	pso_ = PSO::GatInstance();

	textureManager_ = TextureManager::GetInstance();
	// 色のデータを変数から読み込み
	materialData->color = material.color;
	materialData->shininess = 0.5f;
	directionalLightData->direction = dire.direction;
	//directionalLightData->direction =  Normalize(directionalLightData->direction);
	//directXCommon_->GetCommandList()->SetGraphicsRootSignature(pso_->GetProperty().rootSignature.Get());
	//directXCommon_->GetCommandList()->SetPipelineState(pso_->GetProperty().graphicsPipelineState.Get());    //PSOを設定
	directXCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);    //VBVを設定
	//形状を設定。PSOに設定しているものとはまた別。同じものを設定すると考えておけば良い
	//directXCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// マテリアルCBufferの場所を設定
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// SRV のDescriptorTableの先頭を設定。2はrootParameter[2]である。
	directXCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(2, SRVManager::GetGPUDescriptorHandle(texture));
	directXCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	directXCommon_->GetCommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}