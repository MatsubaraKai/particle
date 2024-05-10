#include "Camera.h"


void Camera::Initialize() {
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-80.0f} };
	//WinAPI* sWinAPI = WinAPI::GetInstance();

	cameraMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, asepectRatio_, nearClip_, farClip_);
	//worldCameraMatrix = Multiply(worldmatrix, Multiply(viewMatrix, projectionMatrix));
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

void Camera::Update() {
	cameraMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(fovY_, asepectRatio_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);


}

