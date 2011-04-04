
#include "MenuPageBase.h"
#include "MenuManager.h"

#include <string.h>

MenuManager* MenuManager::menuManager;

void MenuManager::initialize()
{
    if (menuManager == 0)
    {
        menuManager = new MenuManager();
    }
}

void MenuManager::finalize()
{
    if (menuManager)
    {
        delete menuManager;
        menuManager = 0;
    }
}

MenuManager::MenuManager()
    : currentMenuPage(0)
{
    menuManager = this;
    memset(menuPages, 0, sizeof(menuPages));
}

MenuManager::~MenuManager()
{
}

void MenuManager::open()
{
}

void MenuManager::close()
{
}


