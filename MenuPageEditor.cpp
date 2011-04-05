
#include "MenuPageEditor.h"
#include "TheGame.h"
#include "stdafx.h"

#include "TheEarth.h"

MenuPageEditor::MenuPageEditor()
    : window(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-250, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
        false,
        L"Editor",
        0,
        MI_WINDOW);

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(2,22,42,42),
        window,
        MI_BUTTONREFRESH,
        L"Refresh");

    irr::gui::IGUITabControl* tc = TheGame::getInstance()->getEnv()->addTabControl(
        irr::core::recti(irr::core::position2di(2, 44), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, window->getRelativePosition().getSize().Height - 46)),
        window,
        true,
        true,
        MI_TABCONTROL);

    irr::gui::IGUITab* tabTiles = tc->addTab(L"Tiles", MI_TABTILES);
    irr::gui::IGUITab* tab2 = tc->addTab(L"Empty", MI_TAB2);

    tableTiles = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabTiles->getRelativePosition().getSize()),
        tabTiles,
        MI_TABLETILES,
        true);

    tableTiles->addColumn(L"#");
    tableTiles->addColumn(L"X");
    tableTiles->addColumn(L"Y");
    tableTiles->addColumn(L"in use");

    window->setVisible(false);
}

MenuPageEditor::~MenuPageEditor()
{
    //window->remove();
    close();
}

bool MenuPageEditor::OnEvent(const irr::SEvent &event)
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::OnEvent()\n");
    if (event.EventType == irr::EET_GUI_EVENT)
    {
        int id = event.GUIEvent.Caller->getID();

        switch (event.GUIEvent.EventType)
        {
            case irr::gui::EGET_ELEMENT_CLOSED:
            {
                switch (id)
                {
                    case MI_WINDOW:
                        dprintf(MY_DEBUG_NOTE, "event on close editor window\n");
                        MenuManager::getInstance()->close();
                        return true;
                        break;
                };
                break;
            }
            case irr::gui::EGET_BUTTON_CLICKED:
            {
                switch (id)
                {
                    case MI_BUTTONREFRESH:
                        dprintf(MY_DEBUG_NOTE, "editor::refreshbutton clicked\n");
                        refresh();
                        return true;
                        break;
                };
                break;
            }
        };
    }
    return false;
}

void MenuPageEditor::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::open()\n");
    window->setVisible(true);
}

void MenuPageEditor::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::close()\n");
    window->setVisible(false);
}

void MenuPageEditor::refresh()
{
    tableTiles->clearRows();

    TheEarth::tileMap_t tileMap = TheEarth::getInstance()->getTileMap();
    unsigned int i = 0;
    for (TheEarth::tileMap_t::const_iterator tit = tileMap.begin();
         tit != tileMap.end();
         tit++, i++)
    {
        irr::core::stringw str;
        
        tableTiles->addRow(i);

        str += i;
        tableTiles->setCellText(i, 0, str.c_str());

        str = L"";
        str += tit->second->getPosX();
        tableTiles->setCellText(i, 1, str.c_str());

        str = L"";
        str += tit->second->getPosY();
        tableTiles->setCellText(i, 2, str.c_str());

        str = L"";
        str += tit->second->getInUse() ? L"true" : L"false";
        tableTiles->setCellText(i, 3, str.c_str());
    }
}
