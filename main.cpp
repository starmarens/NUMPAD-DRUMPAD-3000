#include "menu.hpp"

int main(){
    Menu menu;
  

    menu.displayMenu();

    menu.getInput();
    menu.setInput();

    menu.optionHandler();
}