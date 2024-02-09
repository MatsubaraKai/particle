#include "GameScene.h"
#define _USE_MATH_DEFINES
#include<math.h>

void GameScene::Init()
{
	camera = new Camera;
	camera->Initialize();
	input = Input::GetInstance();
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE;
		sceneTime = 0;
	}

	camera->Update();
	

}
void GameScene::Draw()
{
	car_->Draw(camera);
}
void GameScene::Release() {
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
