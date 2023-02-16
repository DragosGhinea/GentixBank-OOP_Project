#include "../include/controlPanel.h"
#include<iostream>
#include<conio.h>
#include <unistd.h>
#include "../include/menuUtils.h"
#include "../include/menuLeaderboard.h"
using namespace std;

enum Option{
    LoginCustomer=0,
    LoginModerator=1,
    LoginAdministrator=2,
    LoginROOT=3,
    LeaderboardOP=4
};

Option operator++(Option& op){
    switch(op){
        case LoginCustomer:
            op=LoginModerator;
            break;
        case LoginModerator:
            op=LoginAdministrator;
            break;
        case LoginAdministrator:
            op=LoginROOT;
            break;
        case LoginROOT:
            op=LeaderboardOP;
            break;
        default:
            op=LoginCustomer;
    }
    return op;
}

Option operator--(Option& op){
    switch(op){
        case LoginROOT:
            op=LoginAdministrator;
            break;
        case LoginAdministrator:
            op=LoginModerator;
            break;
        case LoginModerator:
            op=LoginCustomer;
            break;
        case LeaderboardOP:
            op=LoginROOT;
            break;
        default:
            op=LeaderboardOP;
    }
    return op;
}

void loadMenu(Option& option){
    cout<<"\n";
    clearConsole();
    cout<<"For menu navigation use arrows or ASWD.\n";
    cout<<"To select an option press ENTER.\n";
    cout<<"To go to a previous page use ESC.\n";
    cout<<"\n\n";

    cout<<"\t\t\t\t\t┌──────────────────────────┐\n";
    cout<<"\t\t\t\t\t│   Main Menu              │\n";
    cout<<"\t\t\t\t\t├──────────────────────────┤\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t│   Customer Login         │\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t├──────────────────────────┤\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t│   Moderator Login        │\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t├──────────────────────────┤\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t│   Administrator Login    │\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t├──────────────────────────┤\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t│   ROOT Login             │\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t├──────────────────────────┤\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t│   Leaderboard            │\n";
    cout<<"\t\t\t\t\t│                          │\n";
    cout<<"\t\t\t\t\t└──────────────────────────┘\n";

        gotoxy(44, 9);
        if(option == LoginCustomer){
            coutColoredText("Customer Login", Colors::RED);
        }
        else
            cout<<"Customer Login";

        gotoxy(44, 13);
        if(option == LoginModerator){
            coutColoredText("Moderator Login", Colors::RED);
        }
        else
            cout<<"Moderator Login";

        gotoxy(44, 17);
        if(option == LoginAdministrator){
            coutColoredText("Administrator Login", Colors::RED);
        }
        else
            cout<<"Administrator Login";

        gotoxy(44, 21);
        if(option == LoginROOT){
            coutColoredText("ROOT Login", Colors::RED);
        }
        else
            cout<<"ROOT Login";

        gotoxy(44, 25);
        if(option == LeaderboardOP){
            coutColoredText("Leaderboard", Colors::RED);
        }
        else
            cout<<"Leaderboard";

    gotoxy(0, 27);
}

void ControlPanel::menu(){
    Option option = LoginCustomer;

    loadMenu(option);

    char key;
    while(1){
        gotoxy(0, 27);
        key=_getch();
        if(key==27){
            clearConsole();
            cout<<"Goodbye! :)";
            usleep(1000000);
            break;
        }
        else if(key==13){
            User* u;
            if(option == LoginCustomer){
                u= getRandomCustomer();
                if(u==NULL)
                    coutColoredText("Error: No customers in the system.", Colors::LIGHT_RED);
                else{
                    login(u);
                    loadMenu(option);
                }
            }
            else if(option == LoginAdministrator){
                u= getRandomAdministrator();
                if(u==NULL)
                    coutColoredText("Error: No administrators in the system.", Colors::LIGHT_RED);
                else{
                    login(u);
                    loadMenu(option);
                }
            }
            else if(option == LoginModerator){
                u= getRandomModerator();
                if(u==NULL)
                    coutColoredText("Error: No moderators in the system.", Colors::LIGHT_RED);
                else{
                    login(u);
                    loadMenu(option);
                }
            }
            else if(option == LoginROOT){
                login(&getRoot());
                loadMenu(option);
            }
            else if(option == LeaderboardOP){
                menuLeaderboard();
                loadMenu(option);
            }
        }
        else if(key==87 || key==119 || key==72 || key==75){
            --option;
        }
        else if(key==83 || key==115 || key==80 || key==77)
            ++option;
        else{
            continue;
        }
        gotoxy(44, 9);
        if(option == LoginCustomer){
            coutColoredText("Customer Login", Colors::RED);
        }
        else
            cout<<"Customer Login";

        gotoxy(44, 13);
        if(option == LoginModerator){
            coutColoredText("Moderator Login", Colors::RED);
        }
        else
            cout<<"Moderator Login";

        gotoxy(44, 17);
        if(option == LoginAdministrator){
            coutColoredText("Administrator Login", Colors::RED);
        }
        else
            cout<<"Administrator Login";

        gotoxy(44, 21);
        if(option == LoginROOT){
            coutColoredText("ROOT Login", Colors::RED);
        }
        else
            cout<<"ROOT Login";

        gotoxy(44, 25);
        if(option == LeaderboardOP){
            coutColoredText("Leaderboard", Colors::RED);
        }
        else
            cout<<"Leaderboard";
    }
}
