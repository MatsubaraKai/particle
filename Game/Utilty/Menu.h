#pragma once
#include "Sprite.h"
class Menu
{
public:
    void Init(uint32_t Texture);
    void ChangeTex(uint32_t Texture);
    void Draw();

    Material material;
private:
    Sprite* menuSprite = nullptr;
    uint32_t MENUMEDItextureHandle;

};

