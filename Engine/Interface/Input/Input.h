#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <cassert>
#include <wrl.h>
#include <Xinput.h>
//Xinput.lib; Xinput9_1_0.lib
#pragma comment(lib, "Xinput.lib")
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

	bool GetJoystickState(XINPUT_STATE &state);
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber"></param>
	/// <returns></returns>
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);

	

	/*XINPUT_STATE GetXInputState() {
		return state;
	}*/

	// namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	HRESULT result;
	// キーボードデバイスの生成
	ComPtr <IDirectInputDevice8> keyboard = nullptr;
	BYTE keys[256];
	BYTE preKeys[256];
	//XINPUT_STATE state;

	
};

