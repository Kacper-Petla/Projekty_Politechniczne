#include "Konsola.h"

void Konsola::wyczyscEkran() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written;
    DWORD cells;
    COORD home = { 0, 0 };

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    cells = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter(hConsole, ' ', cells, home, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, home, &written);
    SetConsoleCursorPosition(hConsole, home);
}

void Konsola::wyczyscLinie() 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;

    COORD start = { 0, csbi.dwCursorPosition.Y };
    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, start, &written);
    SetConsoleCursorPosition(hConsole, start);
}

void Konsola::ustawKursor(int x, int y) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, coord);
}
