
#ifndef MENUMANAGER_H
#define MENUMANAGER_H


class MenuPageBase;

class MenuManager
{
public:
    static void initialize();
    static void finalize();
    
    static MenuManager* getInstance() {return menuManager;}

private:
    static MenuManager* menuManager;

public:
    enum MenuPageId
    {
        MP_EDITOR = 0
        NUMBER_OF_MENUPAGES
    };

private:
    MenuManager();
    ~MenuManager();

public:
    
    void open();
    void close();

private:
    MenuPageBase*   currentMenuPage;
    
    MenuPageBase*   menuPages[NUMBER_OF_MENUPAGES];
};

#endif // MENUMANAGER_H
