#pragma once
#include <vector>

#include "VertexData.h"
#include "MaterialData.h"
#include "Quaternion.h"
#include <unordered_map>
#include <Matrix4x4.h>
struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;

};

template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};
using KeyFrameVector3 = Keyframe<Vector3>;
using KeyFrameQuaternion = Keyframe<Quaternion>;

template <typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>> keyframes;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct AnimationData {
	float duration; // アニメ―ション全体の尺（単位は秒）
	// NodeAnimationの集合。NOde名で開けるようにしておく
	std::unordered_map<std::string, NodeAnimation> nodeAnimations;
};