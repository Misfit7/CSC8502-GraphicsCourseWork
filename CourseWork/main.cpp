#include "../NCLGL/window.h"
#include "URenderer.h"

int main() {
    Window w("Starfiled", 1280, 720, false);
    if (!w.HasInitialised()) {
        return -1;
    }

    URenderer Urenderer(w);
    if (!Urenderer.HasInitialised()) {
        return -1;
    }

    w.LockMouseToWindow(true);
    w.ShowOSPointer(false);

    while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
        Urenderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
        Urenderer.RenderScene();
        Urenderer.SwapBuffers();

        //Urenderer.AutoScene();

        if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
            Shader::ReloadAllShaders();
        }
    }

    return 0;
}