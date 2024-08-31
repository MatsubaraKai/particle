﻿#include "Camera.h"
#include <algorithm>


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
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.0f, 0.7f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.1f, 0.0f, 0.3f, 0.5f));
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
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void Camera::Move()
{
    // キーボードによる移動
    Vector3 move = { 0.0f, 0.0f, 0.0f };
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

    // 目標角度の算出
    if (move.x != 0.0f || move.z != 0.0f)
    {
        angle_ = std::atan2(move.x, move.z);
        transform_.rotate.y = LerpShortAngle(transform_.rotate.y, angle_, 0.1f);
    }
   
    // キーボードによる移動量の更新
    transform_.translate.x += move.x;
    transform_.translate.z += move.z;

    // ゲームパッドの処理
    HandleGamepadMovement();
}

void Camera::HandleGamepadMovement()
{
    XINPUT_STATE joyState;
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
            float cosY = cosf(transform_.rotate.y);
            float sinY = sinf(transform_.rotate.y);
            Vector3 move = {
                moveLeftStick.x * cosY + moveLeftStick.z * sinY,
                0.0f,
                moveLeftStick.z * cosY - moveLeftStick.x * sinY
            };
            transform_.translate.x += move.x;
            transform_.translate.z += move.z;
        }

        // 右スティックによる視野の移動
        HandleRightStick(joyState);
    }
}

void Camera::SetFOV(float fovY) {
    fovY_ = fovY;
}

void Camera::HandleRightStick(const XINPUT_STATE& joyState)
{
    const float lookSensitivity = 0.03f;
    const float rightStickDeadZone = 0.1f;
    if (std::abs(joyState.Gamepad.sThumbRX) > rightStickDeadZone * SHRT_MAX ||
        std::abs(joyState.Gamepad.sThumbRY) > rightStickDeadZone * SHRT_MAX)
    {
        float rightStickX = (float)joyState.Gamepad.sThumbRX / SHRT_MAX * lookSensitivity;
        float rightStickY = (float)joyState.Gamepad.sThumbRY / SHRT_MAX * lookSensitivity;

        transform_.rotate.y += rightStickX;  // Yaw (水平回転)
        transform_.rotate.x -= rightStickY;  // Pitch (垂直回転)

        // ピッチの制限
        const float maxPitch = 45.0f * (float)std::numbers::pi / 180.0f;
        const float minPitch = -45.0f * (float)std::numbers::pi / 180.0f;
        transform_.rotate.x = std::clamp(transform_.rotate.x, minPitch, maxPitch);
    }
}

void Camera::Jump(bool isOnFloor)
{
    ImGui::Begin("aaa");
    ImGui::Text("aa %d", SE);
    ImGui::End();
    if (isJumping) {
        // ジャンプ中の処理
        transform_.translate.y += jumpVelocity;
        jumpVelocity += Gravity;

        if (transform_.translate.y <= -10.0f) {
            // 落下しすぎた場合のリセット処理
            jumpVelocity = 0.0f;
            transform_.translate = { 0.0f, 15.0f, -15.0f };
            transform_.rotate = { -0.2f, 0.0f, 0.0f };
            isJumping = false;
        }
    }
    else if (!isOnFloor) {
        // 地面にいない場合の処理
        transform_.translate.y += jumpVelocity;
        jumpVelocity += Gravity;
        if (transform_.translate.y <= -10.0f) {
            // 落下しすぎた場合のリセット処理
            jumpVelocity = 0.0f;
            transform_.translate = { 0.0f, 15.0f, -15.0f };
            transform_.rotate = { -0.2f, 0.0f, 0.0f };
            isJumping = false;
        }
    }
    if(isOnFloor) {
        // 地面にいる場合
        isJumping = false;
        jumpVelocity = 0.0f;
    }

    // ゲームパッドでジャンプ
    HandleGamepadJump(isOnFloor);
}

void Camera::HandleGamepadJump(bool isOnFloor)
{
    XINPUT_STATE joyState;
    if (Input::GetInstance()->GetJoystickState(joyState))
    {
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && isOnFloor && !isJumping || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && isOnFloor && !isJumping)
        {
            SE++;
            // 地面にいるときのみジャンプを許可
            isJumping = true;
            jumpVelocity = JumpSpeed;
            if (SE == 1) {
                Audio::SoundPlayWave(Audio::GetInstance()->GetIXAudio().Get(), AudioJumphandle_, false, 0.50f);
            }
            if (SE == 2) {
                SE = 0;
            }
           
        }
        
#ifdef _DEBUG
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
        {
            // デバッグ用に地面の上昇
            transform_.translate.y += 1.0f;
            jumpVelocity = 0.0f;
            isJumping = false;
        }
        if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
        {
            // デバッグ用に地面の下降
            transform_.translate.y -= 1.0f;
            jumpVelocity = 0.0f;
            isJumping = false;
        }
#endif
    }
}

float Camera::Face2Face(const Vector3& playerPosition, const Vector3 objectPosition){
    // Y軸の回転角度を計算
    float deltaX = playerPosition.x - objectPosition.x;
    float deltaZ = playerPosition.z - objectPosition.z;
    float rotationY = atan2f(deltaX, deltaZ);
    return rotationY;
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