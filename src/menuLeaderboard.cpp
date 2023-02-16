#include<conio.h>
#include<iostream>
#include "../include/menuLeaderboard.h"
#include "../include/controlPanel.h"
#include "../include/menuUtils.h"

using namespace std;

void loadLeaderboardDisplay(Leaderboard<Account, double, customCopy>& leaderboard, int page){
    cout<<"\n";
    clearConsole();
    cout<<"─────────────────── Top 10 ───────────────────\n";
    int index= (page - 1) * 10 + 1;
    for(shared_ptr<Account>& c : leaderboard.getTop10(page)){
        cout<<"#"<<index++<<":\n";
        cout<<*c;
    }
    cout<<"──────────────────────────────────────────────\n";
    cout << "Page » " << page << endl;
    cout<<"Press ESC to leave the leaderboard."<<endl;
    cout<<"Use arrows or ASWD to change leaderboard pages."<<endl;
}

void menuLeaderboard(){
    int page=1;
    Leaderboard<Account, double, customCopy> accounts;

    for(Customer* u : ControlPanel::getControlPanel()->getCustomers())
        for(shared_ptr<Account>& c : u->getAccounts()){
            accounts+=*c;
        }

    loadLeaderboardDisplay(accounts, page);

    char key;
    while(1){
        key=_getch();
        if(key==27){
            break;
        }
        else if(key==87 || key==119 || key==72 || key==75){
            if(page != 1)
                loadLeaderboardDisplay(accounts, --page);
        }
        else if(key==83 || key==115 || key==80 || key==77){
            loadLeaderboardDisplay(accounts, ++page);
        }
        else{
            continue;
        }

    }
}
