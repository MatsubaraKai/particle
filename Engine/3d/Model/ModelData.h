#pragma once
#include <vector>

#include "VertexData.h"
#include "MaterialData.h"
#include "Quaternion.h"
#include <unordered_map>
#include <Matrix4x4.h>
#include "Skeleton.h"


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
