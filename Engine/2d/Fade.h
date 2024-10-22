#pragma once
#include "Sprite.h"
class Fade
{
public:
	void Init(uint32_t fadeTexture);
    void SetTexture(uint32_t fadeTexture);
    void SetAlpha(float Alpha);
    void Draw();
	void StartFadeOut();
	void StartFadeIn();
	void UpdateFade();
    bool IsFadeOutComplete() const { return fadeOutComplete; }

    Material material;
private:
    Sprite *fadeSprite = nullptr;
    uint32_t FADEtextureHandle;
    
    float alpha = 0.0f;

    bool isFadeIn = false;
    bool isFadingOut = false;
    bool fadeOutComplete = false; // フェードアウト完了フラグ

    float time = 0.0f; // 時間の初期値
    float deltaTime = 0.5f; // 時間の経過量 (例: 0.1秒ごとに色が更新される)

    void UpdateFadeOut();
    void UpdateFadeIn();
};

