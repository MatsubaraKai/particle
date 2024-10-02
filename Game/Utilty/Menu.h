#pragma once
#include "Sprite.h"
class Menu
{
public:
    void Init(uint32_t Texture);
    void ChangeTex(uint32_t Texture);
    void SE();
    void Draw();
    int sen = 1;
private:
    Sprite* menuSprite = nullptr;
    uint32_t MENUMEDItextureHandle;
    uint32_t Audiomenuhandle_;
    Material material;

};

