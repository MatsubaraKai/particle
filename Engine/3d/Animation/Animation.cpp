#include "Animation.h"
#include <cassert>
/**
* @file Animation.cpp
* @brief 3Dモデルのアニメーションデータを管理・処理するクラス
*/
AnimationData Animation::LoadAnimationFile(const std::string& directoryPath, const std::string& filePath)
{
	AnimationData animation;// 今回作るアニメーション
	Assimp::Importer importer;
	std::string filePathA = directoryPath + "/" + filePath;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);// アニメーションがない
	aiAnimation* animationAssimp = scene->mAnimations[0]; // 最初のアニメーションだけ採用。もちろん複数対応するに越したことはない
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond); // 時間の単位を秒に変換
	AnimationCurve<NodeAnimation> keyframes;
	// assimpでは個々のNodeのAnimationをchannelと読んでいるのでchannelを回してNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyFrameVector3 keyframe{};
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyFrameQuaternion keyframe{};
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyFrameVector3 keyframe{};
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond); // ここも秒に変換
			keyframe.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}

	}
	return animation;
}

void Animation::Init(const std::string& directoryPath, const std::string& filePath)
{
	animation_ = LoadAnimationFile(directoryPath, filePath);

}

void Animation::Updata()
{
	animationTime += 1.0f / 60.0f;
	animationTime = std::fmod(animationTime, animation_.duration); // 最後まで行ったら最初からリピート再生。リピートしなくても別によい
	//NodeAnimation& rootAnimation = animation_.nodeAnimations[mode]
}

//Vector3 Animation::CalucateValue(const std::vector<KeyFrameVector3>& keyframes, float time)
//{
//	assert(!keyframes.empty()); // キーがないものは返す値がわからないのでダメ
//	if (keyframes.size() == 1 || time <= keyframes[0].time) {// キーが1つか、時刻がキーフレーム前なら最初の値とする
//		return keyframes[0].value;
//	}
//
//	for (size_t index = 0; index < keyframes.size() - 1; ++index) {
//		size_t nextIndex = index + 1;
//		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
//		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
//			// 範囲内を補間する
//			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
//			return Lerp();
//		}
//	}
//	
//}
