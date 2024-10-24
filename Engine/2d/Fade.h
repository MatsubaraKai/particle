#pragma once
#include "Sprite.h"
/**
* @file Fade.h
* @brief 画面のフェード効果を管理するクラス
*/
class Fade
{
public:
    /// <summary>
    /// フェードの初期化処理
    /// </summary>
    /// <param name="fadeTexture">フェードに使用するテクスチャハンドル</param>
    void Init(uint32_t fadeTexture);

    /// <summary>
    /// フェード用のテクスチャを設定する
    /// </summary>
    /// <param name="fadeTexture">設定するテクスチャハンドル</param>
    void SetTexture(uint32_t fadeTexture);

    /// <summary>
    /// フェードのアルファ値を設定する
    /// </summary>
    /// <param name="Alpha">設定するアルファ値 (0.0f 〜 1.0f)</param>
    void SetAlpha(float Alpha);

    /// <summary>
    /// フェードの描画処理
    /// </summary>
    void Draw();

    /// <summary>
    /// フェードアウトの開始
    /// </summary>
    void StartFadeOut();

    /// <summary>
    /// フェードインの開始
    /// </summary>
    void StartFadeIn();

    /// <summary>
    /// フェードの更新処理
    /// </summary>
    void UpdateFade();

    /// <summary>
    /// フェードアウトが完了しているかどうかを確認する
    /// </summary>
    /// <returns>フェードアウトが完了している場合は true、そうでない場合は false</returns>
    bool IsFadeOutComplete() const { return fadeOutComplete; }

private:
    /// <summary>
    /// フェードアウトの更新処理
    /// </summary>
    void UpdateFadeOut();

    /// <summary>
    /// フェードインの更新処理
    /// </summary>
    void UpdateFadeIn();

    Sprite* fadeSprite = nullptr;    // フェード用のスプライト
    uint32_t FADEtextureHandle;      // フェード用のテクスチャハンドル
    Material material;               // フェード用のマテリアル
    float alpha = 0.0f;              // 現在のアルファ値

    bool isFadeIn = false;           // フェードイン中かどうか
    bool isFadingOut = false;        // フェードアウト中かどうか
    bool fadeOutComplete = false;    // フェードアウト完了フラグ

    float time = 0.0f;               // フェードの経過時間
    float deltaTime = 0.5f;          // フェードの時間経過速度
};