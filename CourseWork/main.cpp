#include "../NCLGL/window.h"
#include "URenderer.h"
#include "GRenderer.h"

int main() {
    Window w("Starfiled", 1280, 720, false);
    if (!w.HasInitialised()) {
        return -1;
    }

    OGLRenderer* renderer = nullptr;
    URenderer* Urenderer = nullptr;
    GRenderer* Grenderer = new GRenderer(w);
    renderer = Grenderer;
    if (!renderer->HasInitialised()) {
        return -1;
    }

    w.LockMouseToWindow(true);
    w.ShowOSPointer(false);

    while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
        renderer->UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
        renderer->SwapBuffers();

        //ground
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F1)) {
            if (Urenderer) { delete Urenderer; };
            Grenderer = new GRenderer(w);
            renderer = Grenderer;
        }
        //space
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F2)) {
            if (Grenderer) { delete Grenderer; };
            Urenderer = new URenderer(w);
            renderer = Urenderer;
        }

        //reload
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
            Shader::ReloadAllShaders();
        }
    }

    return 0;
}