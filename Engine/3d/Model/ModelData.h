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

struct EulerTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};

struct VATData {
	float VATTime;
	float MaxVATTime;
	Vector4 VatPositionTexSize;//(1.0/width, 1.0/height, width, height)
	Vector4 VatNormalTexSize;//(1.0/width, 1.0/height, width, height)
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
};
struct VertexWeightData {
	float weight;
	uint32_t vertexIndex;
};

struct JointWeightData {
	Matrix4x4 inverseBindPoseMatrix;
	std::vector<VertexWeightData> vertexWeights;
};
struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};
struct ModelData {
	std::vector<VertexData> vertices;
	std::vector<uint32_t> indices;
	std::map<std::string, JointWeightData> skinClusterData;
	MaterialData material;
	uint32_t textureIndex;
	Node rootNode;
	std::string directoryPath;
	std::string filename;

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

