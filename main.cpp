#include "WinAPI.h"
#include "GameManager.h"



// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	GameManager* gameManager = new GameManager();
	gameManager->Run();
	return 0;
}