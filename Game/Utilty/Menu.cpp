#include "Menu.h"
#include "Audio.h"

void Menu::Init(uint32_t Texture)
{
    Audiomenuhandle_ = Audio::SoundLoadWave("Resources/game/Audio/menu.wav");
    MENUPOSITIONtextureHandle = TextureManager::StoreTexture("Resources/game/menuposition.png");
    MENUMEDItextureHandle = Texture;
    menuSprite = new Sprite();
    menuSprite2 = new Sprite();
    menuSprite->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/menumedi.png");
    menuSprite2->Init({ 0.0f,0.0f }, { 1280.0f,720.0f }, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, "Resources/menuposition.png");
    menuSprite->SetTextureSize({ 1280.0f,720.0f });
    menuSprite2->SetTextureSize({ 1280.0f,720.0f });
    material.color = { 1.0f,1.0f,1.0f,1.0f };
    material.enableLighting = true;
}
void Menu::ChangeTex(uint32_t Texture){
    MENUMEDItextureHandle = Texture;
}
void Menu::Draw() {
    menuSprite->Draw(MENUMEDItextureHandle, material.color);
    menuSprite2->Draw(MENUPOSITIONtextureHandle, material.color);
}
void Menu::SE() {
    Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), Audiomenuhandle_, false, 0.4f);
}

