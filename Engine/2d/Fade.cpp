#include "Fade.h"
#include "ImGuiCommon.h"
#include "TextureManager.h"
#define M_PI 3.141592
void Fade::Init(uint32_t fadeTexture)
{
	fadeTex = fadeTexture;
	fadeSprite = new Sprite();
	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/black.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });
	material.color = { 1.0f,1.0f,1.0f,0.0f };
	material.enableLighting = true;
}

void Fade::Draw() {

    fadeSprite->Draw(fadeTex, material.color);
}
void Fade::StartFadeOut()
{
    alpha = 1.0f;
    isFadingOut = true;
    isFadeIn = false;
    fadeOutComplete = false;
}

void Fade::StartFadeIn()
{
    alpha = 0.0f;
    isFadeIn = true;
    isFadingOut = false;
    fadeOutComplete = false;
}

void Fade::UpdateFade()
{
    time += deltaTime; // 時間を進める
    //setRainbowColor(time,material.color.x,material.color.y,material.color.z);
    if (isFadingOut)
    {
        UpdateFadeOut();
    }
    else if (isFadeIn)
    {
        UpdateFadeIn();
    }

    material.color = { 1.0f, 1.0f, 1.0f, alpha };
    //material.color.w = alpha;
}

void Fade::UpdateFadeOut()
{
    if (alpha > 0.0f)
    {
        alpha -= 0.01f; // フェード速度
        if (alpha < 0.0f)
        {
            alpha = 0.0f;
            isFadingOut = false;
        }
    }
}

void Fade::UpdateFadeIn()
{
    if (alpha < 1.0f)
    {
        alpha += 0.01f; // フェード速度
        if (alpha > 1.0f)
        {
            alpha = 1.0f;
            isFadeIn = false;
            fadeOutComplete = true;
        }
    }
}

void Fade::setRainbowColor(float time, float &red, float &green, float &blue) {
    float frequency = 0.5f; // 色の変化の速度を調整するための周波数
    float amplitude = 0.5f; // 色の振幅を調整する係数

    red = amplitude * std::sin(frequency * time + 0) + 0.5f;
    green = amplitude * std::sin(frequency * time + 2 * static_cast<float>(M_PI) / 3) + 0.5f;
    blue = amplitude * std::sin(frequency * time + 4 * static_cast<float>(M_PI) / 3) + 0.5f;
}