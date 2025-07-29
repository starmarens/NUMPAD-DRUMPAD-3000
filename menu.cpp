#include <iostream>
#include <filesystem>
#include <thread>
#include <string>
#include "menu.hpp"
#ifdef _WIN32
#include <windows.h>
#endif


using namespace std;



Menu::Menu(){
    choice = 0;
}

int Menu::getInput(){
   int choice;
    cout << "what option would you like to choose" << endl;
    cin >> choice;

    return choice;

}

void Menu::setInput(){
    this -> choice = Menu::getInput();

}

void Menu::displayMenu() {
    cout << "hello, Welcome to NUMPAD DRUMPAD 3000" << endl;
    //this_thread::sleep_for(1000);
    cout << "1. display numpad" << endl;
    cout << "2. configure numpad" << endl;
    cout << "3. view audio file list" << endl;
    cout << "4. add audio files" << endl;
}

void Menu::displayFiles(){
    int i = 0;
    for(auto entry : filesystem::directory_iterator("/Users/georgelopez/Desktop/NUMPAD DRUMPAD 3000/samples")){
        cout << i << ". " << entry.path().filename() << endl;
        i++;
    }
}

void Menu::optionHandler(){
    switch(this -> choice){
    
    case 1:

        break;
    case 2:

        break;
    case 3:
        this->displayFiles();

        break;
    
    case 4:

        break;
    
}
    
}
