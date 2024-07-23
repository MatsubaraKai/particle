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
    //#ifdef _DEBUG
    ImGui::Begin("camera");
    ImGui::DragFloat("FOV", &fovY_, 0.01f);
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
    //#endif // _DEBUG
}

void Camera::Move()
{
    // ゲームパッドの状態を得る変数(XINPUT)
    XINPUT_STATE joyState;
    Vector3 move = { 0.0f, 0.0f, 0.0f };

    // キーボードによる移動
    if (Input::GetInstance()->PushKey(DIK_W))
    {
        move.z = PlayerSpeed;
    }
    if (Input::GetInstance()->PushKey(DIK_S))
    {
        move.z = -PlayerSpeed;
    }
    if (Input::GetInstance()->PushKey(DIK_A))
    {
        move.x = -PlayerSpeed;
    }
    if (Input::GetInstance()->PushKey(DIK_D))
    {
        move.x = PlayerSpeed;
    }

    // 目標角度の算出（キーボード）
    if (move.x != 0.0f || move.z != 0.0f)
    {
        angle_ = std::atan2(move.x, move.z);
        transform_.rotate.y = LerpShortAngle(transform_.rotate.y, angle_, 0.1f);
    }

    // キーボードによる移動量の更新
    transform_.translate.x += move.x;
    transform_.translate.z += move.z;

    // ジャンプの処理
    if (isJumping)
    {
        transform_.translate.y += jumpVelocity;
        jumpVelocity += Gravity;
        if (transform_.translate.y <= GroundLevel)
        {
            transform_.translate.y = GroundLevel;
            isJumping = false;
            jumpVelocity = 0.0f;
        }
    }

    // ゲームパッドの状態取得
    if (Input::GetInstance()->GetJoystickState(joyState))
    {
        // 左スティックによる移動
        Vector3 moveLeftStick = { 0, 0, 0 };
        const float leftStickDeadZone = 0.2f;
        if (std::abs(joyState.Gamepad.sThumbLX) > leftStickDeadZone * SHRT_MAX ||
            std::abs(joyState.Gamepad.sThumbLY) > leftStickDeadZone * SHRT_MAX)
        {
            moveLeftStick = {
                (float)joyState.Gamepad.sThumbLX / SHRT_MAX,
                0.0f,
                (float)joyState.Gamepad.sThumbLY / SHRT_MAX
            };

            float inputMagnitude = Length(moveLeftStick);
            if (inputMagnitude > leftStickDeadZone)
            {
                moveLeftStick.x *= PlayerSpeed;
                moveLeftStick.z *= PlayerSpeed;
            }
        }

        // カメラの向きに基づく移動方向の調整
        if (moveLeftStick.x != 0.0f || moveLeftStick.z != 0.0f)
        {
            // 移動ベクトルをカメラの向きに合わせる
            float cosY = cosf(transform_.rotate.y);
            float sinY = sinf(transform_.rotate.y);
            move = {
                moveLeftStick.x * cosY + moveLeftStick.z * sinY,
                0.0f,
                moveLeftStick.z * cosY - moveLeftStick.x * sinY
            };
            transform_.translate.x += move.x;
            transform_.translate.z += move.z;
        }

        // 右スティックによる視野の移動
        const float lookSensitivity = 0.03f;
        const float rightStickDeadZone = 0.1f;
        if (std::abs(joyState.Gamepad.sThumbRX) > rightStickDeadZone * SHRT_MAX ||
            std::abs(joyState.Gamepad.sThumbRY) > rightStickDeadZone * SHRT_MAX)
        {
            float rightStickX = (float)joyState.Gamepad.sThumbRX / SHRT_MAX * lookSensitivity;
            float rightStickY = (float)joyState.Gamepad.sThumbRY / SHRT_MAX * lookSensitivity;

            transform_.rotate.y += rightStickX;  // Yaw (水平回転)

            // ピッチに制限をつける (-45度から45度の範囲)
            transform_.rotate.x -= rightStickY;  // Pitch (垂直回転)
            const float maxPitch = 45.0f * (float)std::numbers::pi / 180.0f;
            const float minPitch = -45.0f * (float)std::numbers::pi / 180.0f;
            if (transform_.rotate.x > maxPitch)
            {
                transform_.rotate.x = maxPitch;
            }
            else if (transform_.rotate.x < minPitch)
            {
                transform_.rotate.x = minPitch;
            }
        }

        // Aボタンでジャンプ
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && !isJumping)
        {
            isJumping = true;
            jumpVelocity = JumpSpeed;
        }
#ifdef _DEBUG

        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
        {
            transform_.translate.y += 0.1f;
            isJumping = false;
        }
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
        {
            transform_.translate.y -= 0.1f;
            isJumping = false;
        }

#endif // DEBUG

        
    }
}


float Camera::Lerp(const float& a, const float& b, float t) {
	float result{};

	result = a + b * t;

	return result;
}

// 最短角度補間
float Camera::LerpShortAngle(float a, float b, float t)
{
    float diff = b - a;
    diff = std::fmod(diff + (float)std::numbers::pi, 2 * (float)std::numbers::pi) - (float)std::numbers::pi;
    return a + diff * t;
}

float Camera::LerpShortTranslate(float a, float b, float t) {
	return a + t * (b - a);
}

// 長さ(ノルム)
float Camera::Length(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
};