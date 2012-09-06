#pragma once
#include <windows.h>

struct ConsoleColor {
    ~ConsoleColor();
    ConsoleColor(WORD color);
private:
    bool ColorSave();
    WORD m_color;
};
