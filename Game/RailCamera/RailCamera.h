#pragma once
#include "Camera.h"
#include <memory>

class RailCamera
{
public:
	void Init();
	void Update();

public: // getter
	Camera* GetCamera() { return camera_.get(); }

private:
	std::unique_ptr<Camera> camera_ = nullptr;

};

