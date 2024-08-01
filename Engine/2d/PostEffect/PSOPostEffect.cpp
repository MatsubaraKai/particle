#include "PSOPostEffect.h"
#include "GrayScale.h"
#include "Vignette.h"
#include "DepthOutline.h"
#include "Dissolve.h"
#include "FullScreen.h"
#include "GaussianBlur.h"
#include "RadialBlur.h"
#include "Random.h"
#include "Bloom.h"
#include <d3dx12.h>



void PSOPostEffect::Init()
{
	currentEffectNo_ = IPostEffectState::GetEffectNo();
	effectArr_[PostEffectMode::kFullScreen] = std::make_unique<FullScreen>();
	effectArr_[PostEffectMode::kGrayscale] = std::make_unique<GrayScale>();
	effectArr_[PostEffectMode::kVignetting] = std::make_unique<Vignette>();
	effectArr_[PostEffectMode::kGaussianFilter] = std::make_unique<GaussianBlur>();
	effectArr_[PostEffectMode::kDissolve] = std::make_unique<Dissolve>();
	effectArr_[PostEffectMode::kOutline] = std::make_unique<DepthOutline>();
	effectArr_[PostEffectMode::kRadialBlur] = std::make_unique<RadialBlur>();
	effectArr_[PostEffectMode::kRandom] = std::make_unique<Random>();
	effectArr_[PostEffectMode::kBloom] = std::make_unique<Bloom>();
	effectArr_[currentEffectNo_]->Init();
	property = effectArr_[currentEffectNo_]->CreatePipelineStateObject();
}

void PSOPostEffect::EffectChangeCheck()
{
	// シーンのチェック
	prevEffectNo_ = currentEffectNo_;
	currentEffectNo_ = IPostEffectState::GetEffectNo();

	// シーン変更チェック
	if (prevEffectNo_ != currentEffectNo_) {
		effectArr_[currentEffectNo_]->Init();
		property = effectArr_[currentEffectNo_]->CreatePipelineStateObject();
	}
}

void PSOPostEffect::SetCommand(PostProcess* postProcess)
{
	effectArr_[currentEffectNo_]->CommandRootParameter(postProcess);
}


PSOPostEffect* PSOPostEffect::GatInstance() {
	static PSOPostEffect instance;
	return &instance;
}

void PSOPostEffect::SetEffect(int num) {
	effectArr_[num]->Init();

}