
#ifndef MENUMANAGER_H
#define MENUMANAGER_H


#define MAX_MENU_ITEMS 100

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
        MP_EDITOR = 0,
        NUMBER_OF_MENUPAGES
    };

private:
    MenuManager();
    ~MenuManager();

    void refreshEventReceiver();

public:
    
    void open(MenuPageId menuPageId);
    void close();
    void closeAll();

    bool isInMenu(); // inline

private:
    MenuPageBase*   currentMenuPage;
    
    MenuPageBase*   menuPages[NUMBER_OF_MENUPAGES];
};


inline bool MenuManager::isInMenu()
{
    return (currentMenuPage != 0);
}

#endif // MENUMANAGER_H
