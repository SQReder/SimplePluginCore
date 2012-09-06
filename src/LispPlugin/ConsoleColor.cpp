#include "consolecolor.h"

ConsoleColor::~ConsoleColor() {
    if (m_color)
        ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), m_color);
}

ConsoleColor ::ConsoleColor(WORD color): m_color(0) {
    if (color && ColorSave())
        ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool ConsoleColor ::ColorSave() {
    CONSOLE_SCREEN_BUFFER_INFO tmp;
    if (::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &tmp)) {
        m_color = tmp.wAttributes;
    }
    return m_color;
}
