#pragma once
#include <Windows.h>
#include <cstdint>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

/*----------------------------------------------------------
	   このクラスはシングルトンパターンを元に設計する
--------------------------------------------------------------*/
class WinAPI final
{
public: // 静的メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数

	// コンストラクタ
	WinAPI() = default;
	//デストラクタ
	~WinAPI() = default;

	/*-----Default Method-----*/
	// 初期化
	void Initialize(const wchar_t* label);
	// 解放処理
	void Finalize();

	/*-----User Method-----*/
	// シングルトンパターンの実装
	// コピーコンストラクタを無効にする
	WinAPI(const WinAPI& obj) = delete;
	// 代入演算子を無効にする
	WinAPI& operator=(const WinAPI& obj) = delete;

	// Accessor
	static WinAPI* GetInstance();

	static int32_t GetKClientWidth() { return kClientWidth_; };
	static int32_t GetKClientHeight() { return kClientHeight_; };

	 HWND GetHwnd() { return hwnd_; }

	 WNDCLASS GetWc() { return wc_; }

	
	// ウィンドウの生成
	HWND hwnd_;

	//ウィンドウクラス
	WNDCLASS wc_{};

	// メッセージの処理
	bool ProcessMessage();

public: // 定数
	// クライアント領域のサイズ
	const static  int32_t kClientWidth_ = 1280;
	const static  int32_t kClientHeight_ = 720;
private:

	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController_;
	

};




