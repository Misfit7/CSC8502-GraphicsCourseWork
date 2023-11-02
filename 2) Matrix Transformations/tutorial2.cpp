#include "../nclgl/window.h"
#include "Renderer.hpp"

int main() {
    Window w("Vertex Transformation!", 800, 600, false);
    if (!w.HasInitialised()) {
        return -1;
    }

    Renderer renderer(w);
    if (!renderer.HasInitialised()) {
        return -1;
    }

    float scale = 100.0f;
    float rotation = 0.0f;
    Vector3 position(0, 0, -1000.0f);

    while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
            renderer.SwitchToOrthographic();
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
            renderer.SwitchToPerspective();

        if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS))  ++scale;
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) --scale;

        if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))  ++rotation;
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) --rotation;

        if (Window::GetKeyboard()->KeyDown(KEYBOARD_K))
            position.y -= 10.0f;
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_I))
            position.y += 10.0f;

        if (Window::GetKeyboard()->KeyDown(KEYBOARD_J))
            position.x -= 10.0f;
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_L))
            position.x += 10.0f;

        if (Window::GetKeyboard()->KeyDown(KEYBOARD_O))
            position.z -= 10.0f;
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_P))
            position.z += 10.0f;

        renderer.SetRotation(rotation);
        renderer.SetScale(scale);
        renderer.SetPosition(position);
        renderer.RenderScene();
        renderer.SwapBuffers();

        renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
        renderer.RenderScene();
    }

    return 0;
}