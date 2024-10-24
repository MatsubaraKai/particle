#include "Fade.h"
#define M_PI 3.141592
/**
* @file Fade.cpp
* @brief Fade
*/
void Fade::Init(uint32_t fadeTexture)
{
	FADEtextureHandle = fadeTexture;
	fadeSprite = new Sprite();
	fadeSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/black.png");
	fadeSprite->SetTextureSize({ 1280.0f,720.0f });
	material.color = { 1.0f,1.0f,1.0f,0.0f };
	material.enableLighting = true;
}

void Fade::SetTexture(uint32_t fadeTexture) {
    FADEtextureHandle = fadeTexture;
}
void Fade::SetAlpha(float Alpha) {
    alpha = Alpha;
}

void Fade::Draw() {

    fadeSprite->Draw(FADEtextureHandle, material.color);
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
    if (isFadingOut)
    {
        UpdateFadeOut();
    }
    else if (isFadeIn)
    {
        UpdateFadeIn();
    }

    material.color = { 1.0f, 1.0f, 1.0f, alpha };
}

void Fade::UpdateFadeOut()
{
    if (alpha > 0.0f)
    {
        alpha -= 0.0013f; // フェード速度
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
