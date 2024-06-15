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
void Camera::CameraDebug()
{
#ifdef _DEBUG
	ImGui::Begin("camera");

	float translate[3] = { transform_.translate.x,transform_.translate.y,transform_.translate.z };
	ImGui::DragFloat3("transform", translate, 0.01f);
	float rotate[3] = { transform_.rotate.x,transform_.rotate.y,transform_.rotate.z };
	ImGui::DragFloat3("rotate", rotate, 0.01f);
	float scale[3] = { transform_.scale.x, transform_.scale.y, transform_.scale.z };
	ImGui::DragFloat3("scale", scale, 0.01f);


	transform_.translate = { translate[0],translate[1],translate[2] };
	transform_.rotate = { rotate[0],rotate[1],rotate[2] };
	transform_.scale = { scale[0],scale[1],scale[2] };


	ImGui::End();
#endif // _DEBUG
}