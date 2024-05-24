#pragma once
#include <vector>
#include "Transform.h"
#include "VertexData.h"
#include "MaterialData.h"
#include "Quaternion.h"
#include <unordered_map>
#include <Matrix4x4.h>
#include "map"
#include "optional"
struct Node {
	QuaternionTransform transform;
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

struct Joint {
	QuaternionTransform transform;// Transform情報
	Matrix4x4 localMatrix;// localMAtrix
	Matrix4x4 skeletonSpaceMatrix;// skeletonSpaceでの変換行列
	std::string name;// 名前
	std::vector<int32_t> children;// 子JointのIndexのリスト。いなければ空
	int32_t index;// 自身のIndex
	std::optional<int32_t> parent;// 親JointのIndex。いなければnull
};

struct Skeleton {
	int32_t root;//RootJointのIndex
	std::map<std::string, int32_t> jointMap;// Joint名とIndexとの辞書
	std::vector<Joint> joints;// 所属しているジョイント
};

