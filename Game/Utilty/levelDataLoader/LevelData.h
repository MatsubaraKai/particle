#pragma once
#include <string>
#include <variant>
#include <vector>
#include "Model.h"

struct Vector {
	float num[3];
};

struct EulerTransform {
	Vector scale;
	Vector rotate;
	Vector translate;
};
class LevelData {
public:
	
	//メッシュデータ
	struct MeshData {
		std::string name;
		std::string flieName;
		EulerTransform transform;
	};

	using ObjectData = MeshData;

public:

	//オブジェクトの情報
	std::vector<ObjectData> objectsData_;

};