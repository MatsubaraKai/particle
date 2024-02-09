#include "WinAPI.h"
#include <Windows.h>
#include<string>

#include<dxgidebug.h>
#pragma comment(lib,"dxguid.lib")


#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx12.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/*----------------------------------------------------------
   このクラスはシングルトンパターンのを元に設計する
--------------------------------------------------------------*/

// 初期化
void WinAPI::Initialize(const wchar_t* label) {
	CoInitializeEx(0, COINIT_MULTITHREADED);
	// ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名（なんでも良い）
	wc_.lpszClassName = label;
	// インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc_);

	// ウィンドウサイズを表す構造体にクライアント領域を入れる。
	RECT wrc = { 0,0,kClientWidth_,kClientHeight_ };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hwnd_ = CreateWindow(
		wc_.lpszClassName,        // 利用するクラス名
		label,					 // タイトルバーの文字（何でも良い）
		WS_OVERLAPPEDWINDOW,	 // よく見るウィンドウスタイル
		CW_USEDEFAULT,			 // 表示X座標（Windowsに任せる）
		CW_USEDEFAULT,			 // 表示Y座標（WindowsOSに任せる）
		wrc.right - wrc.left,	 // ウィンドウ横幅
		wrc.bottom - wrc.top,		 // ウィンドウ縦幅
		nullptr,				 // 親ウィンドウハンドル
		nullptr,				 // メニューハンドル
		wc_.hInstance,			 // インスタンスハンドル
		nullptr);				 // オプション

#ifdef _DEBUG
	debugController_ = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_)))) {
		// デバックレイヤーを有効化する
		debugController_->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);
}
#endif

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
};

// 解放処理
void WinAPI::Finalize() {
	
#ifdef _DEBUG
	debugController_->Release();
#endif
	CloseWindow(hwnd_);
	CoUninitialize();
}

/*-----Accessor-----*/
WinAPI* WinAPI::GetInstance() {
	static WinAPI instance;
	return &instance;
}


LRESULT CALLBACK  WinAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリ終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
};

bool WinAPI::ProcessMessage() {
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
	{
		return true;
	}
	return false;
}




