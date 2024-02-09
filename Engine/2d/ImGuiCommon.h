#pragma once
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx12.h"

class WinAPI;
class DirectXCommon;
class ImGuiCommon
{
public:
	
	void Initialize();
	void Update();
	void Draw();

	void UIParameter();
	void UICreate();
	void Release();

private:

	WinAPI* sWinAPI_ = nullptr;
	DirectXCommon* sDirectXCommon_ = nullptr;
};

