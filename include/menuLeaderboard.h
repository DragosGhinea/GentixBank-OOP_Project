#pragma once
#include "leaderboard.h"
#include "account.h"

struct customCopy{
    Account* operator()(Account& cont){
        return cont.Clone();
    }
};

void loadLeaderboardDisplay(Leaderboard<Account, double, customCopy>& leaderboard, int page);

void menuLeaderboard();