#include "Camera.h"
#include "Window.h"
#include <algorithm>

Camera::Camera(void) {
    yaw = 0.0f;
    pitch = 0.0f;

};

Camera::Camera(float pitch, float yaw, Vector3 position) {
    this->pitch = pitch;
    this->yaw = yaw;
    this->position = position;

}

void Camera::UpdateCamera(float dt) {
    //SetPitch(GetPitch() - (Window::GetMouse()->GetRelativePosition()).y);
    pitch -= (Window::GetMouse()->GetRelativePosition().y);
    yaw -= (Window::GetMouse()->GetRelativePosition().x);
    //SetYaw(GetYaw() - (Window::GetMouse()->GetRelativePosition()).x);

    pitch = std::min(pitch, 90.0f);
    pitch = std::max(pitch, -90.0f);

    if (yaw < 0) {
        yaw += 360.0f;
    }
    if (yaw > 360.0f) {
        yaw -= 360.0f;
    }

    Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));

    Vector3 forward = rotation * Vector3(0, 0, -1);
    Vector3 right = rotation * Vector3(1, 0, 0);

    float speed = 60.0f * dt;

    if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
        position += forward * speed;

    }
    if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
        position -= forward * speed;

    }
    if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
        position -= right * speed;

    }
    if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
        position += right * speed;

    }

    if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
        position.y += speed;

    }
    if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
        position.y -= speed;

    }
}

Matrix4 Camera::BuildViewMatrix() {
    return Matrix4::Rotation(-GetPitch(), Vector3(1, 0, 0)) *
        Matrix4::Rotation(-GetYaw(), Vector3(0, 1, 0)) *
        Matrix4::Translation(-position);
};