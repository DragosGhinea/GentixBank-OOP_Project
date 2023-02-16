#include<windows.h>
#include<iostream>
#include "../include/menuUtils.h"
using namespace std;

void color(int ForgC, int BackC){
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}

//https://stackoverflow.com/questions/6486289/how-can-i-clear-console
void clearConsole(){
    #if defined _WIN32
        system("cls");
        //clrscr(); // including header file : conio.h
    #elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");
        //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
    #elif defined (__APPLE__)
        system("clear");
    #endif
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void coutColoredText(string text, int cF, int cB){
    color(cF, cB);
    cout<<text;
    color(Colors::WHITE);
}