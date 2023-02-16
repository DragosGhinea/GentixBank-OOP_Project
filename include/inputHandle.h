#pragma once
#include<limits>
#include<iostream>

template <typename T> void getInput(T &toRead, std::string errorMessage){
    while(1){
        try{
            if(std::cin >> toRead){
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            }
        }catch(...){

        }
        std::cin.clear(); //clear bad input flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << errorMessage << std::endl;
        std::cout<<"Reread=";
    }
}

template <typename T> void getInput(T &toRead, std::string errorMessage, int maxSize){
    while(1){
        try{
            char aux[maxSize];
            std::cin.getline(aux, maxSize, '\n');
            toRead=T(aux);
            return;
        }catch(...){

        }
        std::cin.clear(); //clear bad input flag
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << errorMessage << std::endl;
        std::cout<<"Reread=";
    }
}