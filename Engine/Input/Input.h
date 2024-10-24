#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include <Xinput.h>

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

/**
* @file Input.cpp
* @brief 入力デバイスの管理を行うクラス
*/
class Input
{
public:
    /// <summary>
    /// シングルトンのインスタンスを取得する
    /// </summary>
    /// <returns>Inputクラスのインスタンス</returns>
    static Input* GetInstance();

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Input() = default;

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Input() = default;

    /// <summary>
    /// コピー代入演算子を削除（コピー不可）
    /// </summary>
    /// <param name="other">コピー元のオブジェクト</param>
    /// <returns>コピー不可</returns>
    const Input& operator=(const Input&) = delete;

    /// <summary>
    /// 入力デバイスの初期化を行う
    /// </summary>
    void Initialize();

    /// <summary>
    /// 入力状態の更新を行う
    /// </summary>
    void Update();

    /// <summary>
    /// 入力に関する描画処理を行う（必要に応じて実装）
    /// </summary>
    void Draw();

    /// <summary>
    /// 入力デバイスの解放を行う
    /// </summary>
    void Release();

    /// <summary>
    /// 指定したキーが押されているかをチェックする
    /// </summary>
    /// <param name="keyNumber">チェックするキーのコード（BYTE型）</param>
    /// <returns>キーが押されていればtrue、そうでなければfalse</returns>
    bool PushKey(BYTE keyNumber);

    /// <summary>
    /// 指定したキーが押された瞬間を検出する
    /// </summary>
    /// <param name="keyNumber">チェックするキーのコード（BYTE型）</param>
    /// <returns>キーが新たに押された場合はtrue、そうでなければfalse</returns>
    bool TriggerKey(BYTE keyNumber);

    /// <summary>
    /// ジョイスティックの状態を取得する
    /// </summary>
    /// <param name="state">取得した状態を格納するXINPUT_STATE構造体の参照</param>
    /// <returns>状態の取得に成功した場合はtrue、失敗した場合はfalse</returns>
    bool GetJoystickState(XINPUT_STATE& state);

    // namespace省略のためのエイリアス
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
    /// <summary>
    /// 処理結果を格納するHRESULT
    /// </summary>
    HRESULT result;

    /// <summary>
    /// キーボードデバイスのインターフェース
    /// </summary>
    ComPtr<IDirectInputDevice8> keyboard = nullptr;

    /// <summary>
    /// 現在のキーの押下状態を格納する配列
    /// </summary>
    BYTE keys[256];

    /// <summary>
    /// 前フレームのキーの押下状態を格納する配列
    /// </summary>
    BYTE preKeys[256];
};
