#pragma once
#include "Camera.h"

/**
* @file Object3dCommon.h
* @brief 3Dオブジェクトに共通する処理を管理するクラス
*/
class Object3dCommon
{
private:
    /// <summary>
    /// デフォルトコンストラクタ（シングルトンパターンのためプライベート）
    /// </summary>
    Object3dCommon() = default;

    /// <summary>
    /// デストラクタ（シングルトンパターンのためプライベート）
    /// </summary>
    ~Object3dCommon() = default;

    /// <summary>
    /// コピー代入演算子を削除（コピー不可）
    /// </summary>
    /// <param name="other">代入元のオブジェクト</param>
    /// <returns>コピー不可</returns>
    const Object3dCommon& operator=(const Object3dCommon&) = delete;

public:
    /// <summary>
    /// 初期化処理を行う
    /// </summary>
    void Init();

    /// <summary>
    /// 更新処理を行う
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理を行う
    /// </summary>
    void Draw();

public: // Getter
    /// <summary>
    /// デフォルトカメラを取得する
    /// </summary>
    /// <returns>デフォルトカメラのポインタ</returns>
    Camera* GetDefaultCamera() const { return defaultCamera_; }

    /// <summary>
    /// インスタンスを取得する（シングルトンパターン）
    /// </summary>
    /// <returns>Object3dCommonのインスタンス</returns>
    static Object3dCommon* GetInstance();

public: // Setter
    /// <summary>
    /// デフォルトカメラを設定する
    /// </summary>
    /// <param name="camera">設定するカメラのポインタ</param>
    void SetDefaultCamera(Camera* camera) { this->defaultCamera_ = camera; }

private:
    Camera* defaultCamera_ = nullptr;
};
