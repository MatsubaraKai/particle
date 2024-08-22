#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Model.h"
#include "Object3d.h"
#include "Camera.h"

class LevelData {
public:
	struct ObjectData {
		std::string filename;
		Transform transform;
	};
	std::vector<ObjectData> objects;

private:



};
class Loder
{
public:
	static void LoadJsonFile(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects, Camera* camera);
	static void LoadJsonFile2(const std::string kDefaultBaseDirectory, const std::string fileName, std::vector<Object3d*>& objects);
};

