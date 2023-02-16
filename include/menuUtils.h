#pragma once
#include<string>

enum Colors{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

void coutColoredText(std::string text, int cF, int cB= 0);

void color(int ForgC, int BackC=0);

void gotoxy(int x, int y);

void progressBar(int x, int y);

void clearConsole();