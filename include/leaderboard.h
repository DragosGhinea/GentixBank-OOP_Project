#pragma once
#include<algorithm>
#include<vector>
#include<iostream>
#include<memory>
#include<functional>

template<typename OB>
struct artificialCopy{

    OB* operator()(const OB& toCopy){
        return new OB(toCopy);
    }

};

template<typename OB, typename Cast, typename COPY_F = artificialCopy<OB>>
class Leaderboard{
    COPY_F copyF;
    std::vector<std::shared_ptr<OB>> objects;
    bool isSorted=true;

    void sortObjects(){
        if(isSorted)
            return;
        isSorted=true;
        sort(objects.begin(), objects.end(), &comparatorIntern);
    }

    static bool comparatorIntern (std::shared_ptr<OB> ob1, std::shared_ptr<OB> ob2){
        return (Cast)*ob1>(Cast)*ob2;
    }

public:

    std::vector<std::shared_ptr<OB>> getTop10(int page=1, bool ascending=false);

    OB& findTopElement(int topPoz, bool ascending=false);

    Leaderboard& operator+=(OB& ob){
        objects.push_back(std::shared_ptr<OB>(copyF(ob)));
        isSorted=false;
        return *this;
    }

    Leaderboard& operator-=(OB& ob){
        auto it = objects.begin();
        while(it != objects.end()){
            if(**it == ob) {
                objects.erase(it);
                break;
            }
        }
        return *this;
    }

};

template<typename OB, typename Cast, typename COPY_F>
std::vector<std::shared_ptr<OB>> Leaderboard<OB, Cast, COPY_F>::getTop10(int page, bool ascending){
    sortObjects();
    std::vector<std::shared_ptr<OB>> toReturn;

    page--;
    int index= page * 10;

    if(index > objects.size())
        return toReturn;

    if(ascending){
        for(auto poz= objects.rbegin() + index; poz < objects.rbegin() + index + 10 && poz != objects.rend(); poz++){
            toReturn.push_back(*poz);
        }
    }
    else{
        for(auto poz= objects.begin() + index; poz < objects.begin() + index + 10 && poz != objects.end(); poz++)
            toReturn.push_back(*poz);
    }

    return toReturn;
}

template<typename OB, typename Cast, typename COPY_F>
OB& Leaderboard<OB, Cast, COPY_F>::findTopElement(int index, bool ascending){
    sortObjects();
    if(ascending)
        return **objects[objects.size() - 1 - index];
    return **objects[index];
}