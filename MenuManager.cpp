
#include "MenuPageBase.h"
#include "MenuManager.h"
#include "TheGame.h"

#include "MenuPageEditor.h"

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
    menuPages[MP_EDITOR] = new MenuPageEditor();
}

MenuManager::~MenuManager()
{
    for (unsigned int i = 0; i < NUMBER_OF_MENUPAGES; i++)
    {
        if (menuPages[i])
        {
            delete menuPages[i];
            menuPages[i] = 0;
        }
    }
}

void MenuManager::open(MenuPageId menuPageId)
{
    if (menuPages[menuPageId] && !menuPages[menuPageId]->isOpened())
    {
        if (menuPages[menuPageId]->openMenu(currentMenuPage))
        {
            currentMenuPage = menuPages[menuPageId];
            refreshEventReceiver();
        }
    }
}

void MenuManager::close()
{
    if (currentMenuPage)
    {
        currentMenuPage = currentMenuPage->closeMenu();
        refreshEventReceiver();
    }
}

void MenuManager::closeAll()
{
    while (currentMenuPage)
    {
        currentMenuPage = currentMenuPage->closeMenu();
    }
}

void MenuManager::refreshEventReceiver()
{
    TheGame::getInstance()->getEnv()->setUserEventReceiver(currentMenuPage);
}
