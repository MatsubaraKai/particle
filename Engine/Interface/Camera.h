#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Transform.h"
#include"mathFunction.h"
#include <numbers>

class Camera
{
public:
	Transform cameraTransform_;
	Matrix4x4 worldmatrix_;
	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;
	Matrix4x4 worldViewProjectionMatrix_;

	void Initialize();
	void Update();
	void Draw();
	void SetCameraPosition(const Vector3& position) {
		cameraTransform_.translate = position;
	}
};

