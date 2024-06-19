#pragma once
#include"WinAPI.h"
#include "imgui.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "mathFunction.h"
#include <numbers>

class Camera
{
public:
	void Initialize();
	void Update();

public: //Getter
	const Transform& GetTransform() const { return transform_; }
	const Vector3& GetTranslate() const { return transform_.translate; }
	const Vector3& GetRotate() const { return transform_.rotate; }
	const Vector3& GetScale() const { return transform_.scale; }
	const Matrix4x4& GetCameraMatrix() const { return cameraMatrix_; }
	const Matrix4x4& GetViewMatrix() const { return viewMatrix_; }
	const Matrix4x4& GetProjectionMatrix() const { return projectionMatrix_; }
	const Matrix4x4& GetViewprojectionMatrix() const { return viewProjectionMatrix_; }

public: //Setter
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetScale(const Vector3& scale) { transform_.scale = scale; }
	void SetFovY() {}
	void SetAspectRario() {}
	void SetNearClip() {}
	void SetFarClip() {}
	void CameraDebug();

private:
	Transform transform_;
	Matrix4x4 worldmatrix_;
	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;
	float fovY_ = 0.45f;
	float asepectRatio_ = float(WinAPI::kClientWidth_) / float(WinAPI::kClientHeight_);
	float nearClip_ = 0.1f;
	float farClip_ = 100.0f;
};