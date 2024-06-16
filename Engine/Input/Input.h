#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include "stdint.h"
#include "Xinput.h"
//#include <Xinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:
	static Input* GetInstance();
	Input() = default;
	~Input() = default;

	const Input& operator=(const Input&) = delete;

	void Initialize();
	void Update();
	void Draw();
	void Release();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PushKey(BYTE keyNumber);

	bool TriggerKey(BYTE keyNumber);

	void CreateAllKeyDetectionFunctions();

	// ジョイスティックのデッドゾーンを適用する関数
	SHORT ApplyDeadzone(SHORT value, SHORT deadzone);

	bool GetJoystickState(int32_t stickNo, XINPUT_STATE& state);

	//マウスの左クリックの検知
	bool IsLeftMouseClicked();

	bool IsLeftMouseTrigger();

	// namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	HRESULT result;
	// キーボードデバイスの生成
	ComPtr <IDirectInputDevice8> keyboard = nullptr;
	IDirectInput8* directInput = nullptr;
	BYTE keys[256];
	BYTE preKeys[256];

	const int DEADZONE_THRESHOLD = 8000;
	bool lastState;
	static Input* instance;

};