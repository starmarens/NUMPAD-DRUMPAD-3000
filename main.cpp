#include "menu.hpp"
#include "serial.hpp"

int main(){
    Menu menu;
    Serial serial;

    serial.findCOMports();

    menu.displayMenu();

    menu.getInput();
    menu.setInput();

    menu.optionHandler();
}