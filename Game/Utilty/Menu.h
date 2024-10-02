#pragma once
#include "Sprite.h"
class Menu
{
public:
    void Init(uint32_t Texture);
    void ChangeTex(uint32_t Texture);
    void SE();
    void Draw();
private:
    Sprite* menuSprite = nullptr;
    Sprite* menuSprite2 = nullptr;
    uint32_t MENUMEDItextureHandle;
    uint32_t MENUPOSITIONtextureHandle;
    uint32_t Audiomenuhandle_;
    Material material;

};

