#include "../include/menuUtils.h"
#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

void progressBar(int x, int y){
    float progres=0.0;
    int barL=70;
    while(progres < 1.0){
        gotoxy(x,y);
        int colorB;
        if(progres<0.2){
            //color(4);
            colorB=Colors::RED;
        }
        else if(progres<0.6){
            //color(15);
            colorB=Colors::YELLOW;
        }
        else{
            color(2);
            colorB=Colors::GREEN;
        }
        color(Colors::DARK_GRAY, 0);
        cout<<"█ ";
        int pos=barL*progres;
        color(0, colorB);
        int i=1;
        for(;i<barL;i++){
            if(i<=pos)
                cout<<" ";
            else{
                break;
            }
        }
        color(Colors::DARK_GRAY, 0);
        for(;i<barL;i++){
            cout<<".";
        }
        color(Colors::DARK_GRAY, 0);
        cout<<" █ ";
        color(colorB, 0);
        cout<<int(progres*100.0)<<"%";
        Sleep(50);
        progres+=0.01;
    }

    gotoxy(x,y);
    color(Colors::DARK_GRAY, 0);
    cout<<"█ ";
    int pos=barL*progres;
    color(0, Colors::GREEN);
    int i=1;
    for(;i<barL;i++){
        if(i<=pos)
            cout<<" ";
        else{
            break;
        }
    }
    color(Colors::DARK_GRAY, 0);
    cout<<" █ ";
    color(Colors::GREEN, 0);
    cout<<" Done! ";
    color(Colors::WHITE, 0);
}
