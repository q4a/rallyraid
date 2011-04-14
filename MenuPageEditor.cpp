
#include "MenuPageEditor.h"
#include "TheGame.h"
#include "stdafx.h"

#include "TheEarth.h"
#include "ObjectWire.h"
#include "ObjectPool.h"
#include "ObjectPoolManager.h"
#include "RaceManager.h"
#include "Race.h"
#include "Settings.h"

MenuPageEditor::MenuPageEditor()
    : window(0),
      tableTiles(0),
      tableObjectWire(0),
      tableObjectWireTiles(0),
      tableObjectPool(0),
      tableRaceManager(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-300, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
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

    // ----------------------------
    // Tiles tab
    // ----------------------------
    irr::gui::IGUITab* tabTiles = tc->addTab(L"Tls", MI_TABTILES);

    tableTiles = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabTiles->getRelativePosition().getSize()),
        tabTiles,
        MI_TABLETILES,
        true);

    tableTiles->addColumn(L"#");
    tableTiles->addColumn(L"X");
    tableTiles->addColumn(L"Y");
    tableTiles->addColumn(L"in use");

    // ----------------------------
    // ObjectWire tab
    // ----------------------------
    irr::gui::IGUITab* tabObjectWire = tc->addTab(L"OW_G", MI_TABOBJECTWIRE);

    tableObjectWire = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabObjectWire->getRelativePosition().getSize()),
        tabObjectWire,
        MI_TABLEOBJECTWIRE,
        true);

    tableObjectWire->addColumn(L"#");
    tableObjectWire->addColumn(L"pos");
    tableObjectWire->addColumn(L"objects");

    // ----------------------------
    // ObjectWireTiles tab
    // ----------------------------
    irr::gui::IGUITab* tabObjectWireTiles = tc->addTab(L"OW_T", MI_TABOBJECTWIRETILES);

    tableObjectWireTiles = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabObjectWireTiles->getRelativePosition().getSize()),
        tabObjectWireTiles,
        MI_TABLEOBJECTWIRETILES,
        true);

    tableObjectWireTiles->addColumn(L"#");
    tableObjectWireTiles->addColumn(L"X");
    tableObjectWireTiles->addColumn(L"Y");
    tableObjectWireTiles->addColumn(L"objects");

    // ----------------------------
    // ObjectPool tab
    // ----------------------------
    irr::gui::IGUITab* tabObjectPool = tc->addTab(L"OPool", MI_TABOBJECTPOOL);

    tableObjectPool = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabObjectPool->getRelativePosition().getSize()),
        tabObjectPool,
        MI_TABLEOBJECTPOOL,
        true);

    tableObjectPool->addColumn(L"#");
    tableObjectPool->addColumn(L"name");
    tableObjectPool->addColumn(L"cat");
    tableObjectPool->addColumn(L"num");
    tableObjectPool->addColumn(L"in use");
    tableObjectPool->addColumn(L"in cache");

    // ----------------------------
    // RaceManager tab
    // ----------------------------
    irr::gui::IGUITab* tabRaceManager = tc->addTab(L"RM", MI_TABRACEMANAGER);

    tableRaceManager = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabRaceManager->getRelativePosition().getSize()),
        tabRaceManager,
        MI_TABLERACEMANAGER,
        true);

    tableRaceManager->addColumn(L"#");
    tableRaceManager->addColumn(L"name");
    tableRaceManager->addColumn(L"long name");

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

    // ----------------------------
    // Tiles
    // ----------------------------
    tableTiles->clearRows();

    const TheEarth::tileMap_t& tileMap = TheEarth::getInstance()->getTileMap();
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

    // ----------------------------
    // ObjectWire
    // ----------------------------
    tableObjectWire->clearRows();

    const ObjectWire::globalObjectWire_t& globalObjectWire = ObjectWire::getInstance()->globalObjectWire;
    i = 0;
    for (ObjectWire::globalObjectWire_t::const_iterator owit = globalObjectWire.begin();
         owit != globalObjectWire.end();
         owit++, i++)
    {
        irr::core::stringw str;
        
        tableObjectWire->addRow(i);

        str += i;
        tableObjectWire->setCellText(i, 0, str.c_str());

        str = L"";
        str += owit->first;
        tableObjectWire->setCellText(i, 1, str.c_str());

        str = L"";
        str += owit->second.size();
        tableObjectWire->setCellText(i, 2, str.c_str());
    }

    // ----------------------------
    // ObjectWireTiles
    // ----------------------------
    tableObjectWireTiles->clearRows();

    i = 0;
    const unsigned int objectWireNum = Settings::getInstance()->objectWireNum;
    while (i < objectWireNum*objectWireNum)
    {
        irr::core::stringw str;
        
        tableObjectWireTiles->addRow(i);

        str += i;
        tableObjectWireTiles->setCellText(i, 0, str.c_str());

        str = L"";
        str += ObjectWire::getInstance()->tiles[i]->rpos.X;
        tableObjectWireTiles->setCellText(i, 1, str.c_str());

        str = L"";
        str += ObjectWire::getInstance()->tiles[i]->rpos.Y;
        tableObjectWireTiles->setCellText(i, 2, str.c_str());

        str = L"";
        str += ObjectWire::getInstance()->tiles[i]->objectList.size();
        tableObjectWireTiles->setCellText(i, 3, str.c_str());

        i++;
    }

    // ----------------------------
    // ObjectPool
    // ----------------------------
    tableObjectPool->clearRows();

    const ObjectPoolManager::objectPoolMap_t& objectPoolMap = ObjectPoolManager::getInstance()->getObjectPoolMap();
    i = 0;
    for (ObjectPoolManager::objectPoolMap_t::const_iterator opit = objectPoolMap.begin();
         opit != objectPoolMap.end();
         opit++, i++)
    {
        irr::core::stringw str;
        
        tableObjectPool->addRow(i);

        str += i;
        tableObjectPool->setCellText(i, 0, str.c_str());

        str = L"";
        str += opit->first.c_str();
        tableObjectPool->setCellText(i, 1, str.c_str());

        str = L"";
        str += opit->second->category;
        tableObjectPool->setCellText(i, 2, str.c_str());

        str = L"";
        str += opit->second->num;
        tableObjectPool->setCellText(i, 3, str.c_str());

        str = L"";
        str += opit->second->inUse;
        tableObjectPool->setCellText(i, 4, str.c_str());

        str = L"";
        str += opit->second->objectList.size();
        tableObjectPool->setCellText(i, 5, str.c_str());
    }

    // ----------------------------
    // RaceManager
    // ----------------------------
    tableRaceManager->clearRows();

    const RaceManager::raceMap_t& raceMap = RaceManager::getInstance()->getRaceMap();
    i = 0;
    for (RaceManager::raceMap_t::const_iterator rit = raceMap.begin();
         rit != raceMap.end();
         rit++, i++)
    {
        irr::core::stringw str;
        
        tableRaceManager->addRow(i);

        str += i;
        tableRaceManager->setCellText(i, 0, str.c_str());

        str = L"";
        str += rit->first.c_str();
        tableRaceManager->setCellText(i, 1, str.c_str());

        str = L"";
        str += rit->second->getLongName().c_str();
        tableRaceManager->setCellText(i, 2, str.c_str());
    }

}
