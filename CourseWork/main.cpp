#include "../NCLGL/window.h"
#include "URenderer.h"
#include "GRenderer.h"

int main() {
    Window w("Starfiled", 1280, 720, false);
    if (!w.HasInitialised()) {
        return -1;
    }

    OGLRenderer* renderer;
    URenderer* Urenderer = new URenderer(w);
    GRenderer* Grenderer;
    renderer = Urenderer;
    if (!Urenderer->HasInitialised()) {
        return -1;
    }

    w.LockMouseToWindow(true);
    w.ShowOSPointer(false);

    bool autoPlay = true;

    while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
        renderer->UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
        renderer->RenderScene();
        renderer->SwapBuffers();
        if (autoPlay) { renderer->AutoScene(); };

        //space
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F1)) {
            delete Grenderer;
            Urenderer = new URenderer(w);
            renderer = Urenderer;
        }
        //ground
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F2)) {
            delete Urenderer;
            Grenderer = new GRenderer(w);
            renderer = Grenderer;
        }

        //stop autoPlay
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
            autoPlay = false;
        }

        //reload
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
            Shader::ReloadAllShaders();
        }
    }

    return 0;
}