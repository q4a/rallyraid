
#include "MenuPageEditor.h"
#include "TheGame.h"
#include "stdafx.h"
#include "WStringConverter.h"
#include "ConfigDirectory.h"

#include "TheEarth.h"
#include "ObjectWire.h"
#include "ObjectPool.h"
#include "ObjectPoolManager.h"
#include "RaceManager.h"
#include "Race.h"
#include "Settings.h"
#include "RoadManager.h"
#include "RoadTypeManager.h"
#include "Road.h"
#include "Day.h"
#include "Stage.h"
#include "RoadType.h"
#include "RoadTypeManager.h"

MenuPageEditor* MenuPageEditor::menuPageEditor = 0;

MenuPageEditor::MenuPageEditor()
    : window(0),
      tableSelected(0),
      tableTiles(0),
      tableObjectWire(0),
      tableObjectWireTiles(0),
      tableObjectPool(0),
      tableRaceManager(0),
      tableRoadManagerG(0),
      tableRoadManagerS(0),
      tableRoadManagerV(0),
      tableRoadTypes(0),
      tableRoads(0),
      editBoxNewRoadFilename(0),
      editBoxNewRoadName(0),
      editBoxNewRoadDataFilename(0)
{
    menuPageEditor = this;

    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-350, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
        false,
        L"Editor",
        0,
        MI_WINDOW);

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(2,22,42,42),
        window,
        MI_BUTTONREFRESH,
        L"Refresh");

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(44,22,84,42),
        window,
        MI_BUTTONCREATEROAD,
        L"new road");

    irr::gui::IGUITabControl* tc = TheGame::getInstance()->getEnv()->addTabControl(
        irr::core::recti(irr::core::position2di(2, 44), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, window->getRelativePosition().getSize().Height - 46)),
        window,
        true,
        true,
        MI_TABCONTROL);

    // ----------------------------
    // Selected tab
    // ----------------------------
    irr::gui::IGUITab* tabSelected = tc->addTab(L"S", MI_TABSELECTED);

    tableSelected = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabSelected->getRelativePosition().getSize()),
        tabSelected,
        MI_TABLESELECTED,
        true);

    tableSelected->addColumn(L"item");
    tableSelected->addColumn(L"selected");

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
    tableTiles->addColumn(L"roads");

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
    irr::gui::IGUITab* tabRaceManager = tc->addTab(L"RaM", MI_TABRACEMANAGER);

    tableRaceManager = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabRaceManager->getRelativePosition().getSize()),
        tabRaceManager,
        MI_TABLERACEMANAGER,
        true);

    tableRaceManager->addColumn(L"#");
    tableRaceManager->addColumn(L"name");
    tableRaceManager->addColumn(L"long name");

    // ----------------------------
    // RoadManager tab
    // ----------------------------
    irr::gui::IGUITab* tabRoadManager = tc->addTab(L"RoM", MI_TABROADMANAGER);

    tableRoadManagerG = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, tabRoadManager->getRelativePosition().getSize().Height/4-2)),
        tabRoadManager,
        MI_TABLEROADMANAGERG,
        true);

    tableRoadManagerG->addColumn(L"#");
    tableRoadManagerG->addColumn(L"name");
    tableRoadManagerG->addColumn(L"type");
    tableRoadManagerG->addColumn(L"loaded");
    tableRoadManagerG->addColumn(L"filename");
    tableRoadManagerG->addColumn(L"data");
    tableRoadManagerG->addColumn(L"size");

    tableRoadManagerS = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, tabRoadManager->getRelativePosition().getSize().Height/4+1), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, tabRoadManager->getRelativePosition().getSize().Height/4-2)),
        tabRoadManager,
        MI_TABLEROADMANAGERS,
        true);

    tableRoadManagerS->addColumn(L"#");
    tableRoadManagerS->addColumn(L"tile");
    tableRoadManagerS->addColumn(L"size");

    tableRoadManagerV = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, (tabRoadManager->getRelativePosition().getSize().Height*2)/4+2), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, tabRoadManager->getRelativePosition().getSize().Height/4-2)),
        tabRoadManager,
        MI_TABLEROADMANAGERV,
        true);

    tableRoadManagerV->addColumn(L"#");
    tableRoadManagerV->addColumn(L"road");
    tableRoadManagerV->addColumn(L"begin");
    tableRoadManagerV->addColumn(L"end");

    tableRoadTypes = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, (tabRoadManager->getRelativePosition().getSize().Height*3)/4+2), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, tabRoadManager->getRelativePosition().getSize().Height/4-2)),
        tabRoadManager,
        MI_TABLEROADTYPES,
        true);

    tableRoadTypes->addColumn(L"#");
    tableRoadTypes->addColumn(L"type");

    // ----------------------------
    // Roads tab
    // ----------------------------
    irr::gui::IGUITab* tabRoads = tc->addTab(L"Roads", MI_TABROADS);

    editBoxNewRoadName = TheGame::getInstance()->getEnv()->addEditBox(L"new road name",
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabRoads->getRelativePosition().getSize().Width, 20)),
        true,
        tabRoads,
        MI_EBNEWROADNAME);

    editBoxNewRoadFilename = TheGame::getInstance()->getEnv()->addEditBox(L"new road filename",
        irr::core::recti(irr::core::position2di(0, 22), irr::core::dimension2di(tabRoads->getRelativePosition().getSize().Width, 20)),
        true,
        tabRoads,
        MI_EBNEWROADFILENAME);

    editBoxNewRoadDataFilename = TheGame::getInstance()->getEnv()->addEditBox(L"new road data filename",
        irr::core::recti(irr::core::position2di(0, 2*22), irr::core::dimension2di(tabRoads->getRelativePosition().getSize().Width, 20)),
        true,
        tabRoads,
        MI_EBNEWROADDATAFILENAME);

    tableRoads = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 3*22), irr::core::dimension2di(tabRoads->getRelativePosition().getSize().Width, tabRoads->getRelativePosition().getSize().Height-(3*22))),
        //irr::core::recti(irr::core::position2di(0, 0), tabRoads->getRelativePosition().getSize()),
        tabRoads,
        MI_TABLEROADS,
        true);

    tableRoads->addColumn(L"#");
    tableRoads->addColumn(L"name");
    tableRoads->addColumn(L"type");
    tableRoads->addColumn(L"loaded");
    tableRoads->addColumn(L"filename");
    tableRoads->addColumn(L"data");
    tableRoads->addColumn(L"size");

    window->setVisible(false);
}

MenuPageEditor::~MenuPageEditor()
{
    //window->remove();
    close();
    menuPageEditor = 0;
}

bool MenuPageEditor::OnEvent(const irr::SEvent &event)
{
    //dprintf(MY_DEBUG_NOTE, "MenuPageEditor::OnEvent()\n");
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
                        //dprintf(MY_DEBUG_NOTE, "event on close editor window\n");
                        MenuManager::getInstance()->close();
                        return true;
                        break;
                    default:
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
                        //dprintf(MY_DEBUG_NOTE, "editor::refreshbutton clicked\n");
                        refresh();
                        return true;
                        break;
                    case MI_BUTTONCREATEROAD:
                    {
                        dprintf(MY_DEBUG_NOTE, "editor::race::newRoad\n");
                        std::string roadName;
                        std::string roadFilename;
                        std::string roadDataFilename;
                        WStringConverter::toString(editBoxNewRoadFilename->getText(), roadFilename);
                        WStringConverter::toString(editBoxNewRoadName->getText(), roadName);
                        WStringConverter::toString(editBoxNewRoadDataFilename->getText(), roadDataFilename);
                        RoadManager::roadMap_t::const_iterator rit = RoadManager::getInstance()->roadMap.find(roadName);
                        if (rit == RoadManager::getInstance()->roadMap.end() &&
                            !roadName.empty() &&
                            RoadTypeManager::getInstance()->editorRoadType)
                        {
                            RoadManager::getInstance()->roadMap[roadName] =
                                new Road(roadFilename, roadName, roadDataFilename, RoadTypeManager::getInstance()->editorRoadType, true);
                            refreshRoads();
                        }
                        return true;
                        break;
                    }
                };
                break;
            }
            case irr::gui::EGET_TABLE_CHANGED:
            {
                switch (id)
                {
                    case MI_TABLEOBJECTPOOL:
                        ObjectPoolManager::getInstance()->editorPool = (ObjectPool*)tableObjectPool->getCellData(tableObjectPool->getSelected(), 0);
                        refreshSelected();
                        return true;
                        break;
                    case MI_TABLERACEMANAGER:
                        RaceManager::getInstance()->editorRace = (Race*)tableRaceManager->getCellData(tableRaceManager->getSelected(), 0);
                        refreshSelected();
                        return true;
                        break;
                    case MI_TABLEROADMANAGERG:
                        RoadManager::getInstance()->editorRoad = (Road*)tableRoadManagerG->getCellData(tableRoadManagerG->getSelected(), 0);
                        refreshSelected();
                        return true;
                        break;
                    case MI_TABLEROADTYPES:
                        RoadTypeManager::getInstance()->editorRoadType = (RoadType*)tableRoadTypes->getCellData(tableRoadTypes->getSelected(), 0);
                        refreshSelected();
                        return true;
                        break;
                    case MI_TABLEROADS:
                        RoadManager::getInstance()->editorRoad = (Road*)tableRoads->getCellData(tableRoads->getSelected(), 0);
                        MenuPageEditor::menuPageEditor->refreshSelected();
                        return true;
                        break;
                };
                break;
            }
            case irr::gui::EGET_TABLE_SELECTED_AGAIN:
            {
                switch (id)
                {
                    case MI_TABLERACEMANAGER:
                        RaceManager::getInstance()->editorRace = (Race*)tableRaceManager->getCellData(tableRaceManager->getSelected(), 0);
                        MenuManager::getInstance()->open(MenuManager::MP_EDITORRACE);
                        return true;
                        break;
                    case MI_TABLESELECTED:
                        switch (tableSelected->getSelected())
                        {
                            case 1:
                                if (RaceManager::getInstance()->editorRace) MenuManager::getInstance()->open(MenuManager::MP_EDITORRACE);
                                break;
                            case 2:
                                if (RaceManager::getInstance()->editorDay) MenuManager::getInstance()->open(MenuManager::MP_EDITORDAY);
                                break;
                            case 3:
                                if (RaceManager::getInstance()->editorStage) MenuManager::getInstance()->open(MenuManager::MP_EDITORSTAGE);
                                break;
                            case 4:
                                if (RoadManager::getInstance()->editorRoad) MenuManager::getInstance()->open(MenuManager::MP_EDITORROAD);
                            case 5:
                                //if (RoadTypeManager::getInstance()->editorRoadType) MenuManager::getInstance()->open(MenuManager::MP_EDITORROADTYPE);
                                break;
                        }
                        return true;
                        break;
                    case MI_TABLEROADS:
                        RoadManager::getInstance()->editorRoad = (Road*)tableRoads->getCellData(tableRoads->getSelected(), 0);
                        MenuPageEditor::menuPageEditor->refreshSelected();
                        MenuManager::getInstance()->open(MenuManager::MP_EDITORROAD);
                        return true;
                        break;
                };
                break;
            }
            case irr::gui::EGET_EDITBOX_CHANGED:
            {
                switch (id)
                {
                    case MI_EBNEWROADNAME:
                        refreshRoadEditBoxes(editBoxNewRoadName->getText());
                }
            }
        };
    }
    return false;
}

void MenuPageEditor::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::open()\n");
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(window);
}

void MenuPageEditor::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::close()\n");
    window->setVisible(false);
}

void MenuPageEditor::refresh()
{
    refreshSelected();
    refreshTiles();
    refreshObjectWire();
    refreshObjectWireTiles();
    refreshObjectPool();
    refreshRaceManager();
    refreshRoadManager();
    refreshRoads();
}

void MenuPageEditor::refreshSelected()
{
    // ----------------------------
    // Selected
    // ----------------------------
    tableSelected->clearRows();

    const TheEarth::tileMap_t& tileMap = TheEarth::getInstance()->getTileMap();
    unsigned int i = 0;
    irr::core::stringw str;
        
    tableSelected->addRow(i);
    str = L"object";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (ObjectPoolManager::getInstance()->editorPool)
    {
        str += ObjectPoolManager::getInstance()->editorPool->getName().c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());

    i = 1;
    tableSelected->addRow(i);
    str = L"race";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (RaceManager::getInstance()->editorRace)
    {
        str += RaceManager::getInstance()->editorRace->getName().c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());
    tableSelected->setCellData(i, 1, (void*)RaceManager::getInstance()->editorRace);

    i = 2;
    tableSelected->addRow(i);
    str = L"day";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (RaceManager::getInstance()->editorDay)
    {
        str += RaceManager::getInstance()->editorDay->getName().c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());
    tableSelected->setCellData(i, 1, (void*)RaceManager::getInstance()->editorDay);

    i = 3;
    tableSelected->addRow(i);
    str = L"stage";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (RaceManager::getInstance()->editorStage)
    {
        str += RaceManager::getInstance()->editorStage->getName().c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());
    tableSelected->setCellData(i, 1, (void*)RaceManager::getInstance()->editorStage);

    i = 4;
    tableSelected->addRow(i);
    str = L"road";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (RoadManager::getInstance()->editorRoad)
    {
        str += RoadManager::getInstance()->editorRoad->getName().c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());
    tableSelected->setCellData(i, 1, (void*)RoadManager::getInstance()->editorRoad);

    i = 5;
    tableSelected->addRow(i);
    str = L"road type";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (RoadTypeManager::getInstance()->editorRoadType)
    {
        str += RoadTypeManager::getInstance()->editorRoadType->getName().c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());
    tableSelected->setCellData(i, 1, (void*)RoadTypeManager::getInstance()->editorRoadType);
}

void MenuPageEditor::refreshTiles()
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

        str = L"";
        str += tit->second->roadRoadChunkList.size();
        tableTiles->setCellText(i, 4, str.c_str());
    }
}

void MenuPageEditor::refreshObjectWire()
{
    // ----------------------------
    // ObjectWire
    // ----------------------------
    tableObjectWire->clearRows();

    const ObjectWire::globalObjectWire_t& globalObjectWire = ObjectWire::getInstance()->globalObjectWire;
    unsigned int i = 0;
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

}

void MenuPageEditor::refreshObjectWireTiles()
{
    // ----------------------------
    // ObjectWireTiles
    // ----------------------------
    tableObjectWireTiles->clearRows();

    unsigned int i = 0;
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

}

void MenuPageEditor::refreshObjectPool()
{
    // ----------------------------
    // ObjectPool
    // ----------------------------
    tableObjectPool->clearRows();

    const ObjectPoolManager::objectPoolMap_t& objectPoolMap = ObjectPoolManager::getInstance()->getObjectPoolMap();
    unsigned int i = 0;
    for (ObjectPoolManager::objectPoolMap_t::const_iterator opit = objectPoolMap.begin();
         opit != objectPoolMap.end();
         opit++, i++)
    {
        irr::core::stringw str;
        
        tableObjectPool->addRow(i);

        str += i;
        tableObjectPool->setCellText(i, 0, str.c_str());
        tableObjectPool->setCellData(i, 0, (void*)opit->second);

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
}

void MenuPageEditor::refreshRaceManager()
{
    // ----------------------------
    // RaceManager
    // ----------------------------
    tableRaceManager->clearRows();

    const RaceManager::raceMap_t& raceMap = RaceManager::getInstance()->getRaceMap();
    unsigned int i = 0;
    for (RaceManager::raceMap_t::const_iterator rit = raceMap.begin();
         rit != raceMap.end();
         rit++, i++)
    {
        irr::core::stringw str;
        
        tableRaceManager->addRow(i);

        str += i;
        tableRaceManager->setCellText(i, 0, str.c_str());
        tableRaceManager->setCellData(i, 0, (void*)rit->second);

        str = L"";
        str += rit->first.c_str();
        tableRaceManager->setCellText(i, 1, str.c_str());

        str = L"";
        str += rit->second->getLongName().c_str();
        tableRaceManager->setCellText(i, 2, str.c_str());
    }
}

void MenuPageEditor::refreshRoadManager()
{
    // ----------------------------
    // RoadManager
    // ----------------------------
    tableRoadManagerG->clearRows();
    /*
    tableRoadManagerG->addColumn(L"#");
    tableRoadManagerG->addColumn(L"name");
    tableRoadManagerG->addColumn(L"type");
    tableRoadManagerG->addColumn(L"loaded");
    tableRoadManagerG->addColumn(L"filename");
    tableRoadManagerG->addColumn(L"data");
    */
    const RoadManager::roadMap_t& roadMap = RoadManager::getInstance()->getRoadMap();
    unsigned int i = 0;
    for (RoadManager::roadMap_t::const_iterator rit = roadMap.begin();
         rit != roadMap.end();
         rit++, i++)
    {
        irr::core::stringw str;
        
        tableRoadManagerG->addRow(i);

        str += i;
        tableRoadManagerG->setCellText(i, 0, str.c_str());
        tableRoadManagerG->setCellData(i, 0, (void*)rit->second);

        str = L"";
        str += rit->second->getName().c_str();
        tableRoadManagerG->setCellText(i, 1, str.c_str());

        str = L"";
        str += rit->second->roadType->getName().c_str();
        tableRoadManagerG->setCellText(i, 2, str.c_str());

        str = L"false";
        if (rit->second->isLoaded())
        {
            str = L"true";
        }
        tableRoadManagerG->setCellText(i, 3, str.c_str());

        str = L"";
        str += rit->second->roadFilename.c_str();
        tableRoadManagerG->setCellText(i, 4, str.c_str());

        str = L"";
        str += rit->second->roadDataFilename.c_str();
        tableRoadManagerG->setCellText(i, 5, str.c_str());

        str = L"";
        str += rit->second->roadPointVector.size();
        tableRoadManagerG->setCellText(i, 6, str.c_str());
    }

    tableRoadManagerS->clearRows();
    /*
    tableRoadManagerS->addColumn(L"#");
    tableRoadManagerS->addColumn(L"tile");
    tableRoadManagerS->addColumn(L"size");
    */
    const RoadManager::stageRoadChunkListMap_t sMap = RoadManager::getInstance()->stageRoadChunkListMap;
    i = 0;
    for (RoadManager::stageRoadChunkListMap_t::const_iterator sit = sMap.begin();
         sit != sMap.end();
         sit++, i++)
    {
        irr::core::stringw str;
        
        tableRoadManagerS->addRow(i);

        str += i;
        tableRoadManagerS->setCellText(i, 0, str.c_str());

        str = L"";
        str += sit->first;
        tableRoadManagerS->setCellText(i, 1, str.c_str());

        str = L"";
        str += sit->second.size();
        tableRoadManagerS->setCellText(i, 2, str.c_str());
    }

    tableRoadManagerV->clearRows();
    /*
    tableRoadManagerV->addColumn(L"#");
    tableRoadManagerV->addColumn(L"road");
    tableRoadManagerV->addColumn(L"begin");
    tableRoadManagerV->addColumn(L"end");
    */
    const roadRoadChunkSet_t cSet = RoadManager::getInstance()->visibleRoadChunkSet;
    i = 0;
    for (roadRoadChunkSet_t::const_iterator cit = cSet.begin();
         cit != cSet.end();
         cit++, i++)
    {
        irr::core::stringw str;
        
        tableRoadManagerV->addRow(i);

        str += i;
        tableRoadManagerV->setCellText(i, 0, str.c_str());

        str = L"";
        str += cit->road->getName().c_str();
        tableRoadManagerV->setCellText(i, 1, str.c_str());

        str = L"";
        str += cit->roadChunk.first;
        tableRoadManagerV->setCellText(i, 2, str.c_str());

        str = L"";
        str += cit->roadChunk.second;
        tableRoadManagerV->setCellText(i, 3, str.c_str());
    }

    tableRoadTypes->clearRows();
    /*
    tableRoadTypes->addColumn(L"#");
    tableRoadTypes->addColumn(L"type");
    */
    const RoadTypeManager::roadTypeMap_t& roadTypeMap = RoadTypeManager::getInstance()->getRoadTypeMap();
    i = 0;
    for (RoadTypeManager::roadTypeMap_t::const_iterator rit = roadTypeMap.begin();
         rit != roadTypeMap.end();
         rit++, i++)
    {
        irr::core::stringw str;
        
        tableRoadTypes->addRow(i);

        str += i;
        tableRoadTypes->setCellText(i, 0, str.c_str());
        tableRoadTypes->setCellData(i, 0, (void*)rit->second);

        str = L"";
        str += rit->second->getName().c_str();
        tableRoadTypes->setCellText(i, 1, str.c_str());
    }

}

void MenuPageEditor::refreshRoads()
{
    refreshRoadEditBoxes(editBoxNewRoadName->getText());

    tableRoads->clearRows();
    /*
    tableRoads->addColumn(L"#");
    tableRoads->addColumn(L"name");
    tableRoads->addColumn(L"type");
    tableRoads->addColumn(L"loaded");
    tableRoads->addColumn(L"filename");
    tableRoads->addColumn(L"data");
    */
    const RoadManager::roadMap_t& roadMap = RoadManager::getInstance()->getRoadMap();
    unsigned int i = 0;
    for (RoadManager::roadMap_t::const_iterator rit = roadMap.begin();
         rit != roadMap.end();
         rit++, i++)
    {
        irr::core::stringw str;
        
        tableRoads->addRow(i);

        str += i;
        tableRoads->setCellText(i, 0, str.c_str());
        tableRoads->setCellData(i, 0, (void*)rit->second);

        str = L"";
        str += rit->second->getName().c_str();
        tableRoads->setCellText(i, 1, str.c_str());

        str = L"";
        str += rit->second->roadType->getName().c_str();
        tableRoads->setCellText(i, 2, str.c_str());

        str = L"false";
        if (rit->second->isLoaded())
        {
            str = L"true";
        }
        tableRoads->setCellText(i, 3, str.c_str());

        str = L"";
        str += rit->second->roadFilename.c_str();
        tableRoads->setCellText(i, 4, str.c_str());

        str = L"";
        str += rit->second->roadDataFilename.c_str();
        tableRoads->setCellText(i, 5, str.c_str());

        str = L"";
        str += rit->second->roadPointVector.size();
        tableRoads->setCellText(i, 6, str.c_str());
    }

}

void MenuPageEditor::refreshRoadEditBoxes(const wchar_t* newRoadName)
{
    irr::core::stringw str;

    str = newRoadName;
    editBoxNewRoadName->setText(str.c_str());

    str = L"data/roads/";
    str += newRoadName;
    editBoxNewRoadFilename->setText(str.c_str());

    str = L"data/roads/data/";
    str += newRoadName;
    editBoxNewRoadDataFilename->setText(str.c_str());

}
