# RayScenes

A library adding an easy way to do scenes for raylib(or really anything)!

Single header, just copy `src/rayscenes.cpp` to your project, and rename it to `rayscenes.hpp`.
You can also just copy `src/rayscenes.cpp` to your source folder and `include/rayscenes.hpp` to your header folder and link the .cpp separately.

Written in for C++20, should support C++17.

[Docs](http://rayscenes.vortetty.tk/doxygen/html/)

## Example usage

```cpp
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#include "raylib.h"
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "rayscenes.hpp"

Rectangle centerRect(Vector2 screenSize, Vector2 size) {
    return (Rectangle) { (screenSize.x - size.x) / 2, (screenSize.y - size.y) / 2, size.x, size.y };
}

void mainDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    ClearBackground({ 255, 0, 0, 255 });

    // Text
    std::string text = "Red!";
    Vector2 size = MeasureTextEx(guiFont, text.c_str(), 20.0, 2.0);
    DrawText(text.c_str(), 512/2-size.x/2, 512/2-size.y/2, 20.0, {0, 255, 0, 255});

    // Cycle scene
    if (GuiButton({0, 0, 20, 512}, GuiIconText(RICON_ARROW_LEFT, "")) && isTop) rsm.setActiveScene("tertiary");
    if (GuiButton({492, 0, 20, 512}, GuiIconText(RICON_ARROW_RIGHT, "")) && isTop) rsm.setActiveScene("secondary");
}
void secondaryDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    ClearBackground({ 0, 255, 0, 255 });

    // Text
    std::string text = "Green!";
    Vector2 size = MeasureTextEx(guiFont, text.c_str(), 20.0, 2.0);
    DrawText(text.c_str(), 512/2-size.x/2, 512/2-size.y/2, 20.0, {255, 0, 0, 255});

    // Cycle scene
    if (GuiButton({0, 0, 20, 512}, GuiIconText(RICON_ARROW_LEFT, "")) && isTop) rsm.setActiveScene("main");
    if (GuiButton({492, 0, 20, 512}, GuiIconText(RICON_ARROW_RIGHT, "")) && isTop) rsm.setActiveScene("tertiary");
}
void tertiaryDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    ClearBackground({ 0, 0, 255, 255 });

    // Text
    std::string text = "Blue";
    Vector2 size = MeasureTextEx(guiFont, text.c_str(), 20.0, 2.0);
    DrawText(text.c_str(), 512/2-size.x/2, 512/2-size.y/2, 20.0, {0, 255, 0, 255});

    // Cycle scene
    if (GuiButton({0, 0, 20, 512}, GuiIconText(RICON_ARROW_LEFT, "")) && isTop) rsm.setActiveScene("secondary");
    if (GuiButton({492, 0, 20, 512}, GuiIconText(RICON_ARROW_RIGHT, "")) && isTop) rsm.setActiveScene("main");
}

int main(int argc, char* argv[]) {
    InitWindow(512, 512, "Rayscenes example");

    rayscenes::rayscenemanager rsm;
    rsm.addScene("main", mainDrawing);
    rsm.addScene("secondary", secondaryDrawing);
    rsm.addScene("tertiary", tertiaryDrawing);

    rsm.setActiveScene("main");

    while (!WindowShouldClose()) {
        BeginDrawing();
        rsm.renderActiveScenes();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
```
