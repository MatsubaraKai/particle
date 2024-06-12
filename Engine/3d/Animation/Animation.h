#pragma once
#include "ModelData.h"

#include "mathFunction.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
struct AnimationData {
	float duration = 0; // アニメ―ション全体の尺（単位は秒）
	// NodeAnimationの集合。NOde名で開けるようにしておく
	std::unordered_map<std::string, NodeAnimation> nodeAnimations;
};
class Animation
{
public:
	// アニメーション読み込み
	AnimationData LoadAnimationFile(const std::string& directoryPath, const std::string& filePath);

	void Init(const std::string& directoryPath, const std::string& filePath);
	void Updata();
	//static Vector3 CalucateValue(const std::vector<KeyFrameVector3>& keyframes, float time);
	Matrix4x4 GetAnimationMatrix() { return matrix_; }

private:
	AnimationData animation_;

	Matrix4x4 matrix_;
	float animationTime = 0.0f;
};

