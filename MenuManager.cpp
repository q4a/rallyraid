
#include "MenuPageBase.h"
#include "MenuManager.h"
#include "TheGame.h"

#include "MenuPageEditor.h"
#include "MenuPageEditorRace.h"
#include "MenuPageEditorDay.h"
#include "MenuPageEditorStage.h"
#include "MenuPageEditorRoad.h"

#include <string.h>

class EmptyEventReceiver : public irr::IEventReceiver
{
public:
    EmptyEventReceiver()
    {
    }

    virtual ~EmptyEventReceiver()
    {
    }

private:
    virtual bool OnEvent(const irr::SEvent &event)
    {
        if (event.EventType == irr::EET_GUI_EVENT && (int)event.GUIEvent.EventType == 0)
        {
            printf("ee gui action\n");
        }
        return true;
    }
};

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
    : currentMenuPage(0), menuInput(false), eer(new EmptyEventReceiver())
{
    menuManager = this;
    memset(menuPages, 0, sizeof(menuPages));
    menuPages[MP_EDITOR] = new MenuPageEditor();
    menuPages[MP_EDITORRACE] = new MenuPageEditorRace();
    menuPages[MP_EDITORDAY] = new MenuPageEditorDay();
    menuPages[MP_EDITORSTAGE] = new MenuPageEditorStage();
    menuPages[MP_EDITORROAD] = new MenuPageEditorRoad();
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
    dprintf(MY_DEBUG_NOTE, "MenuManager::open(): id: %d, p: %p, opened: %d\n", (int)menuPageId, menuPages[menuPageId], menuPages[menuPageId]?menuPages[menuPageId]->isOpened():-1);
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
    menuInput = (currentMenuPage != 0);
    TheGame::getInstance()->getCamera()->setInputReceiverEnabled(!menuInput);
}

void MenuManager::clearEventReceiver()
{
    TheGame::getInstance()->getEnv()->setUserEventReceiver(eer);
    menuInput = false;
    TheGame::getInstance()->getCamera()->setInputReceiverEnabled(true);
}
