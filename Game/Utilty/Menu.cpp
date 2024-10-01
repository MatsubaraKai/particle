#include "Menu.h"
void Menu::Init(uint32_t Texture)
{
    MENUMEDItextureHandle = Texture;
    menuSprite = new Sprite();
    menuSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/menumedi.png");
    menuSprite->SetTextureSize({ 1280.0f,720.0f });
    material.color = { 1.0f,1.0f,1.0f,1.0f };
    material.enableLighting = true;
}
void Menu::ChangeTex(uint32_t Texture)
{
    MENUMEDItextureHandle = Texture;
}
void Menu::Draw() {
    menuSprite->Draw(MENUMEDItextureHandle, material.color);
}

