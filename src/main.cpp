#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "../include/controlPanel.h"
#include "../include/menuUtils.h"
#include "../include/leaderboard.h"

/**************************************
An User is abstract and only exists as:
    - ROOT
    - Administrator
    - Moderator
    - Customer

An Account can be:
    - Business
    - Personal

A Card can be:
    - Virtual
    - Standard
    - Premium


Design Patterns:
    Singleton - controlPanel.h
    Factory - user.h
    Decorator - card.h
    Builder - account.h
******************************************/

using namespace std;

//default password moderator: moderator2
//default password admin: amGoodAdmin1
//default password root: unBreakable1

int main()
{
    clearConsole();
    SetConsoleOutputCP( 65001 ); //for utf-8

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int coloane;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    coloane = csbi.srWindow.Right - csbi.srWindow.Left + 1;



    cout<<"\n";
    cout<<"                 ░██████╗░███████╗███╗░░██╗████████╗██╗██╗░░██╗  ██████╗░░█████╗░███╗░░██╗██╗░░██╗\n";
    cout<<"                 ██╔════╝░██╔════╝████╗░██║╚══██╔══╝██║╚██╗██╔╝  ██╔══██╗██╔══██╗████╗░██║██║░██╔╝\n";
    cout<<"                 ██║░░██╗░█████╗░░██╔██╗██║░░░██║░░░██║░╚███╔╝░  ██████╦╝███████║██╔██╗██║█████═╝░\n";
    cout<<"                 ██║░░╚██╗██╔══╝░░██║╚████║░░░██║░░░██║░██╔██╗░  ██╔══██╗██╔══██║██║╚████║██╔═██╗░\n";
    cout<<"                 ╚██████╔╝███████╗██║░╚███║░░░██║░░░██║██╔╝╚██╗  ██████╦╝██║░░██║██║░╚███║██║░╚██╗\n";
    cout<<"                 ░╚═════╝░╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚═╝╚═╝░░╚═╝  ╚═════╝░╚═╝░░╚═╝╚═╝░░╚══╝╚═╝░░╚═╝\n";


    cout<<"\n";
    for(int i=0;i<coloane/2-7;i++)
        cout<<"─";
    cout<<" Informations ";
    for(int i=0;i<coloane/2-7;i++)
        cout<<"─";

    cout<<"\n\nThrough this command line you will access different options that this bank system offers.\n\n";
    cout<<"The system will randomly generate different type of users\nto which it will associate a random number of accounts and cards.\n";
    cout<<"\n\n";
    cout<<"» Object Structure:\n\n";
    cout<<"       ╔═> Customer  ═╗\n";
    cout<<"User  ═╣              ╠═> Moderator/Administrator\n";
    cout<<"       ╠═> Employee  ═╝\n";
    cout<<"       ╚═> ROOT\n";
    cout<<"\n";
    cout<<"Account (Personal) ═> AccountBusiness\n";
    cout<<"Card (Virtual) ═> CardStandard ═> CardPremium\n\n";

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    srand(time(NULL));
    progressBar(2, 27); //useless but looking good :)
    cout<<"\n\n";
    cout<<"┌───────────────────────────────────────┐\n";
    cout<<"│      Press anything to continue       │\n";
    cout<<"└───────────────────────────────────────┘\n";

    _getch();

    ControlPanel::getControlPanel()->loadRandomData();
    ControlPanel::getControlPanel()->menu();


    return 0;
}

