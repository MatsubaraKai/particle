#include "GameScene.h"
#define _USE_MATH_DEFINES
#include<math.h>

void GameScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	input = Input::GetInstance();
	car_ = new Car();
	car_->Init();
	floor_ = new Floor();
	floor_->Init();
	skydome = new Skydome();
	skydome->Init();
	tree = new Tree();
	tree->Init();
	front_left_tire_ = new Front_left_tire();
	front_left_tire_->Init();
	front_right_tire_ = new Front_right_tire();
	front_right_tire_->Init();
	rear_left_tire_ = new Rear_left_tire();
	rear_left_tire_->Init();
	rear_right_tire_ = new Rear_right_tire();
	rear_right_tire_->Init();

	AccelDriftCamera;

	//SceleCamera
	NormalCamera.scale = { 1.0f,1.0f,1.0f };
	AccelCamera.scale = {2.0f,2.0f,0.6f};
	DriftCamera.scale = { 1.0f,1.0f,1.0f };
	AccelDriftCamera.scale = { 2.0f,2.0f,0.6f };

	DriftCamera.rotate.x = 0.125f;
	NormalCamera.rotate.x = 0.125f;

}

void GameScene::Update()
{
	float theta = (car_->rotate_ / 2.0f) * (float)M_PI;
	Vector2 move = { cosf(theta),sinf(theta) };
	car_->worldTransform_.rotation_.y = theta;
	camera->cameraTransform_.translate.x = car_->worldTransform_.translation_.x;
	camera->cameraTransform_.rotate.y = car_->worldTransform_.rotation_.y / 10;
	camera->cameraTransform_.rotate.z = camera->cameraTransform_.rotate.y;
	sceneTime++;
	if (input->TriggerKey(DIK_W)) {
		moveFlag = true;
	}
	
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
		sceneTime = 0;
	}
	if (camera->cameraTransform_.rotate.y <= -0.15f) {
		camera->cameraTransform_.rotate.y = -0.15f;
	}
	if (camera->cameraTransform_.rotate.y >= 0.15f) {
		camera->cameraTransform_.rotate.y = 0.15f;
	}

	Depart();
	Accel();
	camera->Update();
	floor_->Update();
	car_->Update();
	skydome->Update();
	tree->Update();
	front_left_tire_->Update();
	front_right_tire_->Update();
	rear_left_tire_->Update();
	rear_right_tire_->Update();
	ImGui::Begin("Camera");
	ImGui::Text("NormalCamera");
	ImGui::DragFloat3("NScale",&NormalCamera.scale.x, 0.1f);
	ImGui::DragFloat3("NRotate", &NormalCamera.rotate.x,0.1f);
	ImGui::DragFloat3("NTranslate", &NormalCamera.translate.x);

	ImGui::Text("AccelCamera");
	ImGui::DragFloat3("AScale", &AccelCamera.scale.x, 0.1f);
	ImGui::DragFloat3("ARotate", &AccelCamera.rotate.x);
	ImGui::DragFloat3("ATranslate", &AccelCamera.translate.x);

	ImGui::Text("DriftCamera");
	ImGui::DragFloat3("DScale", &DriftCamera.scale.x, 0.1f);
	ImGui::DragFloat3("DRotate", &DriftCamera.rotate.x);
	ImGui::DragFloat3("DTranslate", &AccelCamera.translate.x);

	ImGui::Text("AccelDriftCamera");
	ImGui::DragFloat3("ADScale", &AccelDriftCamera.scale.x, 0.1f);
	ImGui::DragFloat3("ADRotate", &AccelDriftCamera.rotate.x);
	ImGui::DragFloat3("ADTranslate", &AccelDriftCamera.translate.x);

	ImGui::Checkbox("Aceel",&AccelFlag);
	ImGui::Checkbox("Drift", &DriftFlag);
	ImGui::End();

	if (DriftFlag) {
		car_->SetDriveMode(DriftMode);
	}
	else if (!DriftFlag && !moveFlag) {
		car_->SetDriveMode(NormalMode);
	}
}
void GameScene::Draw()
{
	floor_->Draw(camera);
	car_->Draw(camera);
	skydome->Draw(camera);
	tree->Draw(camera);
	/*front_left_tire_->Draw(camera);
	front_right_tire_->Draw(camera);
	rear_left_tire_->Draw(camera);
	rear_right_tire_->Draw(camera);*/
}
void GameScene::Release() {
	delete floor_;
	delete front_left_tire_;
	delete front_right_tire_;
	delete rear_left_tire_;
	delete rear_right_tire_;
	delete car_;
	delete skydome;
	delete tree;
}
// ゲームを終了
int GameScene::GameClose()
{
	return false;
}

// 車の発車
void GameScene::Depart()
{
	XINPUT_STATE Gamepad{};
	Input::GetInstance()->GetJoystickState(Gamepad);
	if (Gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

		moveFlag = true;

	}

	else {

	}



}



void GameScene::Accel(){
	XINPUT_STATE joyState;
	float theta = (car_->rotate_ / 2.0f) * (float)M_PI;
	Vector2 move = { cosf(theta),sinf(theta) };
	Input::GetInstance()->GetJoystickState(joyState);
	switch (car_->GetDriveMode())
	{
	case NormalMode: {

		//camera->cameraTransform_.rotate = AccelCamera.rotate;
		//camera->cameraTransform_.translate = AccelCamera.translate;
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) || input->PushKey(DIK_LSHIFT) || AccelFlag) {
			if (camera->cameraTransform_.scale.x <= AccelCamera.scale.x) {
				camera->cameraTransform_.scale.x += 0.05f;
				if (camera->cameraTransform_.scale.x > AccelCamera.scale.x) {
					camera->cameraTransform_.scale.x = AccelCamera.scale.x;
				}
			}
			else if (camera->cameraTransform_.scale.x >= AccelCamera.scale.x) {
				camera->cameraTransform_.scale.x -= 0.05f;
				if (camera->cameraTransform_.scale.x < AccelCamera.scale.x) {
					camera->cameraTransform_.scale.x = AccelCamera.scale.x;
				}
			}
			if (camera->cameraTransform_.scale.y <= AccelCamera.scale.y) {
				camera->cameraTransform_.scale.y += 0.05f;
				if (camera->cameraTransform_.scale.y > AccelCamera.scale.y) {
					camera->cameraTransform_.scale.y = AccelCamera.scale.y;
				}
			}
			else if (camera->cameraTransform_.scale.y >= AccelCamera.scale.y) {
				camera->cameraTransform_.scale.y -= 0.05f;
				if (camera->cameraTransform_.scale.y < AccelCamera.scale.y) {
					camera->cameraTransform_.scale.y = AccelCamera.scale.y;
				}
			}
			if (camera->cameraTransform_.scale.z >= AccelCamera.scale.z) {
				camera->cameraTransform_.scale.z -= 0.02f;
				if (camera->cameraTransform_.scale.z < AccelCamera.scale.z) {
					camera->cameraTransform_.scale.z = AccelCamera.scale.z;
				}
			}
			else if (camera->cameraTransform_.scale.z <= AccelCamera.scale.z) {
				camera->cameraTransform_.scale.z += 0.02f;
				if (camera->cameraTransform_.scale.z > AccelCamera.scale.z) {
					camera->cameraTransform_.scale.z = AccelCamera.scale.z;
				}
			}
			/*camera->cameraTransform_.scale.x = 2.0f;
			camera->cameraTransform_.scale.y = 2.0f;
		camera->cameraTransform_.scale.z = 0.6f;*/
			if (moveFlag) {
				camera->cameraTransform_.translate.x += car_->Speed * move.y;
				camera->cameraTransform_.translate.z += car_->Speed * move.x;
			}
		}
		else if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) || !input->PushKey(DIK_LSHIFT))
		{
			//camera->cameraTransform_.rotate = NormalCamera.rotate;
			//camera->cameraTransform_.translate = NormalCamera.translate;
			camera->cameraTransform_.translate.z = car_->GetWorldTransform().z - 25;
			camera->cameraTransform_.translate.y = 6.0f;
			camera->cameraTransform_.rotate.x = NormalCamera.rotate.x;
			if (camera->cameraTransform_.scale.x >= NormalCamera.scale.x) {
				camera->cameraTransform_.scale.x -= 0.05f;
				if (camera->cameraTransform_.scale.x < NormalCamera.scale.x) {
					camera->cameraTransform_.scale.x = NormalCamera.scale.x;
				}
			}
			else if (camera->cameraTransform_.scale.x <= NormalCamera.scale.x) {
				camera->cameraTransform_.scale.x += 0.05f;
				if (camera->cameraTransform_.scale.x > NormalCamera.scale.x) {
					camera->cameraTransform_.scale.x = NormalCamera.scale.x;
				}
			}
			if (camera->cameraTransform_.scale.y >= NormalCamera.scale.y) {
				camera->cameraTransform_.scale.y -= 0.05f;
				if (camera->cameraTransform_.scale.y < NormalCamera.scale.y) {
					camera->cameraTransform_.scale.y = NormalCamera.scale.y;
				}
			}
			else if (camera->cameraTransform_.scale.y <= NormalCamera.scale.y) {
				camera->cameraTransform_.scale.y += 0.05f;
				if (camera->cameraTransform_.scale.y > NormalCamera.scale.y) {
					camera->cameraTransform_.scale.y = NormalCamera.scale.y;
				}
			}
			if (camera->cameraTransform_.scale.z <= NormalCamera.scale.z) {
				camera->cameraTransform_.scale.z += 0.02f;
				if (camera->cameraTransform_.scale.z > NormalCamera.scale.z) {
					camera->cameraTransform_.scale.z = NormalCamera.scale.z;
				}
			}
			else if (camera->cameraTransform_.scale.z >= NormalCamera.scale.z) {
				camera->cameraTransform_.scale.z -= 0.02f;
				if (camera->cameraTransform_.scale.z < NormalCamera.scale.z) {
					camera->cameraTransform_.scale.z = NormalCamera.scale.z;
				}
			}
			if (moveFlag) {
				camera->cameraTransform_.translate.x += car_->Speed * move.y;
				camera->cameraTransform_.translate.z += car_->Speed * move.x;
			}
			/*camera->cameraTransform_.scale.x = 1.0f;
			camera->cameraTransform_.scale.y = 1.0f;
			camera->cameraTransform_.scale.z = 1.0f;*/
		}
		break;
	}
	case DriftMode: {
		if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) || input->PushKey(DIK_LSHIFT) || AccelFlag ) {

			//camera->cameraTransform_.rotate = AccelDriftCamera.rotate;
			//camera->cameraTransform_.translate = AccelDriftCamera.translate;

			camera->cameraTransform_.translate.z = car_->GetWorldTransform().z - 25;
			camera->cameraTransform_.translate.y = 6.0f;
			camera->cameraTransform_.rotate.x = DriftCamera.rotate.x;

			if (camera->cameraTransform_.scale.x <= AccelDriftCamera.scale.x) {
				camera->cameraTransform_.scale.x += 0.05f;
				if (camera->cameraTransform_.scale.x > AccelDriftCamera.scale.x) {
					camera->cameraTransform_.scale.x = AccelDriftCamera.scale.x;
				}
			}
			else if (camera->cameraTransform_.scale.x >= AccelDriftCamera.scale.x) {
				camera->cameraTransform_.scale.x -= 0.05f;
				if (camera->cameraTransform_.scale.x < AccelDriftCamera.scale.x) {
					camera->cameraTransform_.scale.x = AccelDriftCamera.scale.x;
				}
			}
			if (camera->cameraTransform_.scale.y <= AccelDriftCamera.scale.y) {
				camera->cameraTransform_.scale.y += 0.05f;
				if (camera->cameraTransform_.scale.y > AccelDriftCamera.scale.y) {
					camera->cameraTransform_.scale.y = AccelDriftCamera.scale.y;
				}
			}
			else if (camera->cameraTransform_.scale.y >= AccelDriftCamera.scale.y) {
				camera->cameraTransform_.scale.y -= 0.05f;
				if (camera->cameraTransform_.scale.y < AccelDriftCamera.scale.y) {
					camera->cameraTransform_.scale.y = AccelDriftCamera.scale.y;
				}

			}
			if (camera->cameraTransform_.scale.z >= AccelDriftCamera.scale.z) {
				camera->cameraTransform_.scale.z -= 0.02f;
				if (camera->cameraTransform_.scale.z < AccelDriftCamera.scale.z) {
					camera->cameraTransform_.scale.z = AccelDriftCamera.scale.z;
				}
			}
			else if (camera->cameraTransform_.scale.z <= AccelDriftCamera.scale.z) {
				camera->cameraTransform_.scale.z += 0.02f;
				if (camera->cameraTransform_.scale.z > AccelDriftCamera.scale.z) {
					camera->cameraTransform_.scale.z = AccelDriftCamera.scale.z;
				}
			}
			if (moveFlag) {
				camera->cameraTransform_.translate.x += car_->Speed * move.y;
				camera->cameraTransform_.translate.z += car_->Speed * move.x;
			}
			/*camera->cameraTransform_.scale.x = 2.0f;
			camera->cameraTransform_.scale.y = 2.0f;
		camera->cameraTransform_.scale.z = 0.6f;*/
		}
		else if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) || !input->PushKey(DIK_LSHIFT))
		{
			//camera->cameraTransform_.rotate = DriftCamera.rotate;
			//camera->cameraTransform_.translate = DriftCamera.translate;
			camera->cameraTransform_.translate.z = car_->GetWorldTransform().z - 25;
			camera->cameraTransform_.translate.y = 6.0f;
			camera->cameraTransform_.rotate.x = DriftCamera.rotate.x;
			 
			if (camera->cameraTransform_.scale.x >= DriftCamera.scale.x) {
				camera->cameraTransform_.scale.x -= 0.05f;
				if (camera->cameraTransform_.scale.x < DriftCamera.scale.x) {
					camera->cameraTransform_.scale.x = DriftCamera.scale.x;
				}
			}
			else if (camera->cameraTransform_.scale.x <= DriftCamera.scale.x) {
				camera->cameraTransform_.scale.x += 0.05f;
				if (camera->cameraTransform_.scale.x > DriftCamera.scale.x) {
					camera->cameraTransform_.scale.x = DriftCamera.scale.x;
				}
			}
			if (camera->cameraTransform_.scale.y >= DriftCamera.scale.y) {
				camera->cameraTransform_.scale.y -= 0.05f;
				if (camera->cameraTransform_.scale.y < DriftCamera.scale.y) {
					camera->cameraTransform_.scale.y = DriftCamera.scale.y;
				}
			}
			else if (camera->cameraTransform_.scale.y <= DriftCamera.scale.y) {
				camera->cameraTransform_.scale.y += 0.05f;
				if (camera->cameraTransform_.scale.y > DriftCamera.scale.y) {
					camera->cameraTransform_.scale.y = DriftCamera.scale.y;
				}
			}
			if (camera->cameraTransform_.scale.z <= DriftCamera.scale.z) {
				camera->cameraTransform_.scale.z += 0.02f;
				if (camera->cameraTransform_.scale.z > DriftCamera.scale.z) {
					camera->cameraTransform_.scale.z = DriftCamera.scale.z;
				}
			}
			else if (camera->cameraTransform_.scale.z >= DriftCamera.scale.z) {
				camera->cameraTransform_.scale.z -= 0.02f;
				if (camera->cameraTransform_.scale.z < DriftCamera.scale.z) {
					camera->cameraTransform_.scale.z = DriftCamera.scale.z;
				}
			}
			if (moveFlag) {
				camera->cameraTransform_.translate.x += car_->DriftSpeed * move.y;
				camera->cameraTransform_.translate.z += car_->DriftSpeed * move.x;
			}
			/*camera->cameraTransform_.scale.x = 1.0f;
			camera->cameraTransform_.scale.y = 1.0f;
			camera->cameraTransform_.scale.z = 1.0f;*/
		}
		break;
	}
	}

}
