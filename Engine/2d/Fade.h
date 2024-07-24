#pragma once
#include "IScene.h"
#include "Triangle.h"
#include "WorldTransform.h"
#include "Input.h"
#include "Xinput.h"
#include "Particle.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Sprite.h"

class Fade
{
public:
	void Init(uint32_t fadeTexture);
    void Draw();
	void StartFadeOut();
	void StartFadeIn();
	void UpdateFade();
    void setRainbowColor(float time, float &red, float &green, float &blue);
    bool IsFadeOutComplete() const { return fadeOutComplete; }

    Material material;
private:
    Sprite *fadeSprite = nullptr;
    uint32_t fadeTex;
    
    float alpha = 0;

    bool isFadeIn = false;
    bool isFadingOut = false;
    bool fadeOutComplete = false; // フェードアウト完了フラグ

    float time = 0.0f; // 時間の初期値
    float deltaTime = 0.5f; // 時間の経過量 (例: 0.1秒ごとに色が更新される)

    void UpdateFadeOut();
    void UpdateFadeIn();
};

