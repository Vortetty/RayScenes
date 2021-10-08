#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_RICONS
#define GUI_TEXTBOX_EXTENDED_IMPLEMENTATION
#define SUPPORT_FILEFORMAT_BMP
#define SUPPORT_FILEFORMAT_PNG
#define SUPPORT_FILEFORMAT_TGA
#define SUPPORT_FILEFORMAT_JPG
#define SUPPORT_FILEFORMAT_GIF
#define SUPPORT_FILEFORMAT_PSD
#define SUPPORT_FILEFORMAT_PIC
#define SUPPORT_FILEFORMAT_HDR
#define SUPPORT_FILEFORMAT_DDS
#define SUPPORT_FILEFORMAT_PKM
#define SUPPORT_FILEFORMAT_KTX
#define SUPPORT_FILEFORMAT_PVR
#define SUPPORT_FILEFORMAT_ASTC
#include "raylib.h"
#include "raygui.h"
//#include "gui_textbox_extended.h"
#undef RAYGUI_IMPLEMENTATION

#include <string>
#include <rayscenes.hpp>

Rectangle centerRect(Vector2 screenSize, Vector2 size) {
    return (Rectangle) { (screenSize.x - size.x) / 2, (screenSize.y - size.y) / 2, size.x, size.y };
}

void mainDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    ClearBackground(GRAY);

    if (!isTop) GuiDisable();
    GuiDrawIcon(RICON_HOUSE, {0, 0}, 32, DARKGRAY);
    if (GuiButton(centerRect({512, 512}, {128, 64}), "Open Popup") && isTop) rsm.addActiveScene("popup");
    if (GuiButton({0, 0, 20, 512}, GuiIconText(RICON_ARROW_LEFT, "")) && isTop) rsm.setActiveScene("tertiary");
    if (GuiButton({492, 0, 20, 512}, GuiIconText(RICON_ARROW_RIGHT, "")) && isTop) rsm.setActiveScene("secondary");
    if (!isTop) GuiEnable();
}
void popupDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    GuiDrawRectangle({128, 128, 256, 256}, 2, BLACK, {201, 201, 201, 255});
    GuiDrawRectangle({128, 128, 256, 20}, 2, BLACK, {201, 201, 201, 255});
    if (GuiButton({364, 128, 20, 20}, GuiIconText(RICON_CROSS, "")) && isTop) rsm.removeActiveScene("popup");
    GuiDrawRectangle({364, 128, 20, 20}, 2, BLACK, { 0 });
    GuiLabel({132, 130, 16, 16}, "Popup window!!");
    GuiLabel({132, 104, 124, 104}, "Lorem ipsum dolor sit amet, consectetur\nadipiscing elit, sed do eiusmod tempor incididunt\nut labore et dolore magna aliqua. Ut enim ad\nminim veniam, quis nostrud exercitation ullamco\nlaboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in\nvoluptate velit esse cillum dolore eu fugiat\nnulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum.");
}
void secondaryDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    ClearBackground(GRAY);

    GuiDrawIcon(RICON_FILETYPE_INFO, {0, 0}, 32, DARKGRAY);
    std::string text = "This is a secondary window!\nIt was very easy to add\n\nHow to add one:\n - Make a function to render window\n - Register said function\n - Add a call to change to this window\nAnd now we have this!";
    Vector2 size = MeasureTextEx(guiFont, text.c_str(), 20.0, 2.0);
    DrawText(text.c_str(), 512/2-size.x/2, 512/2-size.y/2, 20.0, RAYWHITE);
    if (GuiButton({0, 0, 20, 512}, GuiIconText(RICON_ARROW_LEFT, "")) && isTop) rsm.setActiveScene("main");
    if (GuiButton({492, 0, 20, 512}, GuiIconText(RICON_ARROW_RIGHT, "")) && isTop) rsm.setActiveScene("tertiary");
}
void tertiaryDrawing(rayscenes::rayscenemanager &rsm, bool isTop) {
    ClearBackground(GRAY);

    std::string text = "This is a tertiary window!\nThis was easy to add too!";
    Vector2 size = MeasureTextEx(guiFont, text.c_str(), 20.0, 2.0);
    DrawText(text.c_str(), 512/2-size.x/2, 512/2-size.y/2, 20.0, RAYWHITE);
    if (GuiButton({0, 0, 20, 512}, GuiIconText(RICON_ARROW_LEFT, "")) && isTop) rsm.setActiveScene("secondary");
    if (GuiButton({492, 0, 20, 512}, GuiIconText(RICON_ARROW_RIGHT, "")) && isTop) rsm.setActiveScene("main");
}

int main(int argc, char* argv[]) {
    InitWindow(512, 512, "Rayscenes example");

    rayscenes::rayscenemanager rsm;
    rsm.addScene("main", mainDrawing);
    rsm.addScene("popup", popupDrawing);
    rsm.addScene("secondary", secondaryDrawing);
    rsm.addScene("tertiary", tertiaryDrawing);

    rsm.addActiveScene("main");

    while (!WindowShouldClose()) {
        BeginDrawing();
        rsm.renderActiveScenes();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
