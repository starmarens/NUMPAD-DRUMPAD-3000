#ifndef MENU_H
#define MENU_H

class Menu {
public:
    Menu();
    void setInput();
    int getInput();
    void displayMenu();
    void displayFiles();
    void optionHandler();
private:
    int choice;
    
 
};


#endif
