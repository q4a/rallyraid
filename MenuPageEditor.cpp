
#include "MenuPageEditor.h"
#include "TheGame.h"
#include "stdafx.h"
#include "WStringConverter.h"
#include "ConfigDirectory.h"

#include "TheEarth.h"
#include "ObjectWire.h"
#include "ObjectPool.h"
#include "ObjectPoolManager.h"
#include "ObjectWireGlobalObject.h"
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
#include "ItinerManager.h"
#include "ItinerPoint.h"
#include "AIPoint.h"
#include "WayPointManager.h"
#include "WayPoint.h"

MenuPageEditor* MenuPageEditor::menuPageEditor = 0;

MenuPageEditor::MenuPageEditor()
    : window(0),
      tableSelected(0),
      tableAction(0),
      tableTiles(0),
      tableObjectWire(0),
      tableObjectWireTiles(0),
      tableObjectPool(0),
      tableRaceManager(0),
      tableRoadManagerS(0),
      tableRoadManagerV(0),
      tableRoadTypes(0),
      tableRoads(0),
      tableItiner(0),
      tableItiner2(0),
      editBoxNewRoadFilename(0),
      editBoxNewRoadName(0),
      editBoxNewRoadDataFilename(0),
      editBoxItinerGD(0),
      editBoxItinerLD(0),
      editBoxItinerDescription(0),
      checkBoxRender(0),
      staticTextItinerGD(0),
      currentAction(A_None),
      material(),
      lastTick(0),
      doRender(true)
{
    menuPageEditor = this;
    material.MaterialType = irr::video::EMT_SOLID;
    material.setFlag(irr::video::EMF_LIGHTING, false);
    material.Thickness = 2.0f;

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

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(86,22,146,42),
        window,
        MI_BUTTONACTIVATE,
        L"activate race");

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

    checkBoxRender = TheGame::getInstance()->getEnv()->addCheckBox(doRender,
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabSelected->getRelativePosition().getSize().Width, 20)),
        tabSelected,
        MI_CBRENDER,
        L"render helpers");

    tableSelected = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 44), irr::core::dimension2di(tabSelected->getRelativePosition().getSize().Width, (tabSelected->getRelativePosition().getSize().Height-44)/3-1)),
        tabSelected,
        MI_TABLESELECTED,
        true);

    tableSelected->addColumn(L"item");
    tableSelected->setColumnWidth(0, 50);
    tableSelected->addColumn(L"selected");
    tableSelected->setColumnWidth(1, 150);

    tableAction = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 44+(tabSelected->getRelativePosition().getSize().Height-44)/3+1), irr::core::dimension2di(tabSelected->getRelativePosition().getSize().Width, ((tabSelected->getRelativePosition().getSize().Height-44)*2)/3-1)),
        tabSelected,
        MI_TABLEACTION,
        true);

    tableAction->addColumn(L"Action");
    tableAction->setColumnWidth(0, 200);
    tableAction->addRow(A_None);
    tableAction->addRow(A_AddObjectGlobal);
    tableAction->addRow(A_AddObjectRace);
    tableAction->addRow(A_AddObjectDay);
    tableAction->addRow(A_AddObjectStage);
    tableAction->addRow(A_AddRoadPoint);
    tableAction->addRow(A_AddRoadPointBegin);
    tableAction->addRow(A_AddItinerPoint);
    tableAction->addRow(A_AddAIPoint);
    tableAction->addRow(A_AddWayPoint);
    tableAction->addRow(A_RemoveObjectGlobal);
    tableAction->addRow(A_RemoveObjectRace);
    tableAction->addRow(A_RemoveObjectDay);
    tableAction->addRow(A_RemoveObjectStage);
    tableAction->addRow(A_RemoveRoadPoint);
    tableAction->addRow(A_RemoveRoadPointBegin);
    tableAction->addRow(A_RemoveItinerPoint);
    tableAction->addRow(A_RemoveAIPoint);
    tableAction->addRow(A_RemoveWayPoint);
    tableAction->setCellText(A_None, 0, L"none");
    tableAction->setCellText(A_AddObjectGlobal, 0, L"not used"/*L"add object global"*/);
    tableAction->setCellText(A_AddObjectRace, 0, L"add object race");
    tableAction->setCellText(A_AddObjectDay, 0, L"add object day");
    tableAction->setCellText(A_AddObjectStage, 0, L"add object stage");
    tableAction->setCellText(A_AddRoadPoint, 0, L"add road point");
    tableAction->setCellText(A_AddRoadPointBegin, 0, L"add road point begin");
    tableAction->setCellText(A_AddItinerPoint, 0, L"add itiner point");
    tableAction->setCellText(A_AddAIPoint, 0, L"add AI point");
    tableAction->setCellText(A_AddWayPoint, 0, L"add waypoint");
    tableAction->setCellText(A_RemoveObjectGlobal, 0, L"not used"/*L"remove object global"*/);
    tableAction->setCellText(A_RemoveObjectRace, 0, L"remove object race");
    tableAction->setCellText(A_RemoveObjectDay, 0, L"remove object day");
    tableAction->setCellText(A_RemoveObjectStage, 0, L"remove object stage");
    tableAction->setCellText(A_RemoveRoadPoint, 0, L"remove road point");
    tableAction->setCellText(A_RemoveRoadPointBegin, 0, L"remove road point begin");
    tableAction->setCellText(A_RemoveItinerPoint, 0, L"remove itiner point");
    tableAction->setCellText(A_RemoveAIPoint, 0, L"remove AI point");
    tableAction->setCellText(A_RemoveWayPoint, 0, L"remove waypoint");

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
    tableTiles->setColumnWidth(1, 50);
    tableTiles->addColumn(L"Y");
    tableTiles->setColumnWidth(2, 50);
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
    tableObjectPool->setColumnWidth(1, 80);
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
    tableRaceManager->setColumnWidth(1, 80);
    tableRaceManager->addColumn(L"long name");
    tableRaceManager->setColumnWidth(2, 180);

    // ----------------------------
    // RoadManager tab
    // ----------------------------
    irr::gui::IGUITab* tabRoadManager = tc->addTab(L"RoM", MI_TABROADMANAGER);

    tableRoadManagerS = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, (tabRoadManager->getRelativePosition().getSize().Height)/3-2)),
        tabRoadManager,
        MI_TABLEROADMANAGERS,
        true);

    tableRoadManagerS->addColumn(L"#");
    tableRoadManagerS->addColumn(L"tile");
    tableRoadManagerS->addColumn(L"size");

    tableRoadManagerV = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, ((tabRoadManager->getRelativePosition().getSize().Height)*1)/3+2), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, (tabRoadManager->getRelativePosition().getSize().Height)/3-2)),
        tabRoadManager,
        MI_TABLEROADMANAGERV,
        true);

    tableRoadManagerV->addColumn(L"#");
    tableRoadManagerV->addColumn(L"road");
    tableRoadManagerV->addColumn(L"begin");
    tableRoadManagerV->addColumn(L"end");

    tableRoadTypes = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, ((tabRoadManager->getRelativePosition().getSize().Height)*2)/3+2), irr::core::dimension2di(tabRoadManager->getRelativePosition().getSize().Width, (tabRoadManager->getRelativePosition().getSize().Height)/3-2)),
        tabRoadManager,
        MI_TABLEROADTYPES,
        true);

    tableRoadTypes->addColumn(L"#");
    tableRoadTypes->addColumn(L"type");
    tableRoadTypes->setColumnWidth(1, 100);

    /*
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
    tableRoads->setColumnWidth(1, 60);
    tableRoads->addColumn(L"type");
    tableRoads->setColumnWidth(2, 60);
    tableRoads->addColumn(L"size");
    tableRoads->addColumn(L"loaded");
    tableRoads->setColumnWidth(4, 30);
    tableRoads->addColumn(L"filename");
    tableRoads->addColumn(L"data");
    */

    // ----------------------------
    // Itiner tab
    // ----------------------------
    irr::gui::IGUITab* tabItiner = tc->addTab(L"Iti", MI_TABITINER);

    /*editBoxItinerGD = TheGame::getInstance()->getEnv()->addEditBox(L"global distance",
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, 20)),
        true,
        tabItiner,
        MI_EBITINERGD);
*/
    
    staticTextItinerGD = TheGame::getInstance()->getEnv()->addStaticText(L"0",
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, 20)),
        true,
        false,
        tabItiner,
        MI_STITINERGD);

    editBoxItinerLD = TheGame::getInstance()->getEnv()->addEditBox(L"local distance",
        irr::core::recti(irr::core::position2di(0, 22), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, 20)),
        true,
        tabItiner,
        MI_EBITINERLD);

    editBoxItinerDescription = TheGame::getInstance()->getEnv()->addEditBox(L"description",
        irr::core::recti(irr::core::position2di(0, 2*22), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, 20)),
        true,
        tabItiner,
        MI_EBITINERDESCRIPTION);

    tableItiner = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 3*22), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, tabItiner->getRelativePosition().getSize().Height-(3*22))),
        //irr::core::recti(irr::core::position2di(0, 0), tabRoads->getRelativePosition().getSize()),
        tabItiner,
        MI_TABLEITINER,
        true);

    tableItiner = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 3*22), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, (tabItiner->getRelativePosition().getSize().Height-(3*22))/2 - 1)),
        //irr::core::recti(irr::core::position2di(0, 0), tabRoads->getRelativePosition().getSize()),
        tabItiner,
        MI_TABLEITINER,
        true);

    tableItiner->addColumn(L"itiner image");
    tableItiner->setColumnWidth(0, 200);

    tableItiner2 = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 3*22 + (tabItiner->getRelativePosition().getSize().Height-(3*22))/2 + 1), irr::core::dimension2di(tabItiner->getRelativePosition().getSize().Width, (tabItiner->getRelativePosition().getSize().Height-(3*22))/2-1)),
        //irr::core::recti(irr::core::position2di(0, 0), tabRoads->getRelativePosition().getSize()),
        tabItiner,
        MI_TABLEITINER2,
        true);

    tableItiner2->addColumn(L"itiner image 2");
    tableItiner2->setColumnWidth(0, 200);

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
                    case MI_BUTTONACTIVATE:
                        dprintf(MY_DEBUG_NOTE, "editor::activatebutton clicked\n");
                        if (RaceManager::getInstance()->editorStage)
                        {
                            RaceManager::getInstance()->activateStage(RaceManager::getInstance()->editorStage);
                        }
                        return true;
                        break;
                };
                break;
            }
            case irr::gui::EGET_TABLE_CHANGED:
            {
                switch (id)
                {
                    case MI_TABLEACTION:
                        currentAction = (Action)tableAction->getSelected();
                        return true;
                        break;
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
                    case MI_TABLEITINER:
                    {
                        WStringConverter::toString(tableItiner->getCellText(tableItiner->getSelected(), 0), ItinerManager::getInstance()->editorItinerImageName);
                        MenuPageEditor::menuPageEditor->refreshSelected();
                        return true;
                        break;
                    }
                    case MI_TABLEITINER2:
                    {
                        WStringConverter::toString(tableItiner2->getCellText(tableItiner2->getSelected(), 0), ItinerManager::getInstance()->editorItinerImageName2);
                        MenuPageEditor::menuPageEditor->refreshSelected();
                        return true;
                        break;
                    }
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
                        break;
                }
            }
            case irr::gui::EGET_EDITBOX_ENTER:
            {
                switch (id)
                {
                    case MI_EBITINERGD:
                        WStringConverter::toFloat(editBoxItinerGD->getText(), ItinerManager::getInstance()->editorGlobalDistance);
                        dprintf(MY_DEBUG_INFO, "set itiner global distance: %f\n", ItinerManager::getInstance()->editorGlobalDistance);
                        break;
                    case MI_EBITINERLD:
                        WStringConverter::toFloat(editBoxItinerLD->getText(), ItinerManager::getInstance()->editorLocalDistance);
                        dprintf(MY_DEBUG_INFO, "set itiner local distance: %f\n", ItinerManager::getInstance()->editorLocalDistance);
                        break;
                    case MI_EBITINERDESCRIPTION:
                        WStringConverter::toString(editBoxItinerDescription->getText(), ItinerManager::getInstance()->editorDescription);
                        dprintf(MY_DEBUG_INFO, "set itiner description: [%s]\n", ItinerManager::getInstance()->editorDescription.c_str());
                        break;
                }
            }
            case irr::gui::EGET_CHECKBOX_CHANGED:
                switch (id)
                {
                    case MI_CBRENDER:
                        doRender = checkBoxRender->isChecked();
                        dprintf(MY_DEBUG_INFO, "set render to: %u\n", doRender);
                        break;
                }
        };
    }
    return false;
}

void MenuPageEditor::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::open()\n");
    refresh();
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
    refreshItiner();
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

    i = 6;
    tableSelected->addRow(i);
    str = L"itiner image";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (!ItinerManager::getInstance()->editorItinerImageName.empty())
    {
        str += ItinerManager::getInstance()->editorItinerImageName.c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());

    i = 7;
    tableSelected->addRow(i);
    str = L"itiner image 2";
    tableSelected->setCellText(i, 0, str.c_str());
    str = L"";
    if (!ItinerManager::getInstance()->editorItinerImageName2.empty())
    {
        str += ItinerManager::getInstance()->editorItinerImageName2.c_str();
    }
    else
    {
        str = L"none";
    }
    tableSelected->setCellText(i, 1, str.c_str());
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
    tableRoadManagerS->clearRows();
    /*
    tableRoadManagerS->addColumn(L"#");
    tableRoadManagerS->addColumn(L"tile");
    tableRoadManagerS->addColumn(L"size");
    */
    const RoadManager::stageRoadChunkListMap_t sMap = RoadManager::getInstance()->stageRoadChunkListMap;
    unsigned int i = 0;
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
    /*
    refreshRoadEditBoxes(editBoxNewRoadName->getText());

    tableRoads->clearRows();

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

        str = L"";
        str += rit->second->roadPointVector.size();
        tableRoads->setCellText(i, 3, str.c_str());

        str = L"false";
        if (rit->second->isLoaded())
        {
            str = L"true";
        }
        tableRoads->setCellText(i, 4, str.c_str());

        str = L"";
        str += rit->second->roadFilename.c_str();
        tableRoads->setCellText(i, 5, str.c_str());

        str = L"";
        str += rit->second->roadDataFilename.c_str();
        tableRoads->setCellText(i, 6, str.c_str());
    }
    */
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

void MenuPageEditor::refreshItiner()
{
    tableItiner->clearRows();

    const ItinerManager::itinerImageMap_t& itinerImageMap = ItinerManager::getInstance()->getItinerImageMap();
    unsigned int i = 0;

    irr::core::stringw str;

    tableItiner->addRow(i);
    str = L"none";
    tableItiner->setCellText(i, 0, str.c_str());

    i++;

    for (ItinerManager::itinerImageMap_t::const_iterator it = itinerImageMap.begin();
         it != itinerImageMap.end();
         it++, i++)
    {
        tableItiner->addRow(i);

        str = L"";
        str += it->first.c_str();
        tableItiner->setCellText(i, 0, str.c_str());
    }

    tableItiner2->clearRows();

    const ItinerManager::itinerImageMap_t& itinerImageMap2 = ItinerManager::getInstance()->getItinerImageMap2();
    unsigned int i = 0;

    tableItiner2->addRow(i);
    str = L"none";
    tableItiner2->setCellText(i, 0, str.c_str());

    i++;

    for (ItinerManager::itinerImageMap_t::const_iterator it = itinerImageMap2.begin();
         it != itinerImageMap2.end();
         it++, i++)
    {
        tableItiner2->addRow(i);

        str = L"";
        str += it->first.c_str();
        tableItiner2->setCellText(i, 0, str.c_str());
    }
}

void MenuPageEditor::refreshItinerGD()
{
    irr::core::stringw str;

    str += ItinerManager::getInstance()->editorGlobalDistance;
    staticTextItinerGD->setText(str.c_str());
}

/* static */ void MenuPageEditor::action()
{
    if (menuPageEditor)
    {
        menuPageEditor->actionP();
    }
}

/* static */ void MenuPageEditor::render()
{
    if (menuPageEditor)
    {
        menuPageEditor->renderP();
    }
}

void MenuPageEditor::actionP()
{
    unsigned int tick = TheGame::getInstance()->getTick();

    if (tick - lastTick < 500) return;

    lastTick = tick;

    irr::core::vector3df pos = TheGame::getInstance()->getCamera()->getPosition();
    pos.Y = TheEarth::getInstance()->getHeight(pos);
    printf("action at (%f, %f, %f)\n", pos.X, pos.Y, pos.Z);
    irr::core::vector3df apos = pos + OffsetManager::getInstance()->getOffset();

    switch (currentAction)
    {
    case A_AddObjectRace:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add object race editorRace: %p, editorPool: %p\n",
                RaceManager::getInstance()->editorRace, ObjectPoolManager::getInstance()->editorPool);
            if (RaceManager::getInstance()->editorRace && ObjectPoolManager::getInstance()->editorPool)
            {
                ObjectWireGlobalObject* go = new ObjectWireGlobalObject(ObjectPoolManager::getInstance()->editorPool, apos);
                RaceManager::getInstance()->editorRace->globalObjectList.push_back(go);
                if (RaceManager::getInstance()->editorRace->active)
                {
                    ObjectWire::getInstance()->addGlobalObject(go);
                    //go->setVisible(true);
                }
            }
            break;
        }
    case A_AddObjectDay:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add object day editorDay: %p, editorPool: %p\n",
                RaceManager::getInstance()->editorDay, ObjectPoolManager::getInstance()->editorPool);
            if (RaceManager::getInstance()->editorDay && ObjectPoolManager::getInstance()->editorPool)
            {
                ObjectWireGlobalObject* go = new ObjectWireGlobalObject(ObjectPoolManager::getInstance()->editorPool, apos);
                RaceManager::getInstance()->editorDay->globalObjectList.push_back(go);
                if (RaceManager::getInstance()->editorDay->active)
                {
                    ObjectWire::getInstance()->addGlobalObject(go);
                    //go->setVisible(true);
                }
            }
            break;
        }
    case A_AddObjectStage:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add object stage editorStage: %p, editorPool: %p\n",
                RaceManager::getInstance()->editorStage, ObjectPoolManager::getInstance()->editorPool);
            if (RaceManager::getInstance()->editorStage && ObjectPoolManager::getInstance()->editorPool)
            {
                ObjectWireGlobalObject* go = new ObjectWireGlobalObject(ObjectPoolManager::getInstance()->editorPool, apos);
                RaceManager::getInstance()->editorStage->globalObjectList.push_back(go);
                if (RaceManager::getInstance()->editorStage->active)
                {
                    ObjectWire::getInstance()->addGlobalObject(go);
                    //go->setVisible(true);
                }
            }
            break;
        }
    case A_AddRoadPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add road point, editorRoad: %p\n", RoadManager::getInstance()->editorRoad);
            if (RoadManager::getInstance()->editorRoad)
            {
                RoadManager::getInstance()->editorRoad->addRoadFarPoint(pos);
            }
            break;
        }
    case A_AddRoadPointBegin:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add road point begin, editorRoad: %p\n",
                RoadManager::getInstance()->editorRoad);
            if (RoadManager::getInstance()->editorRoad)
            {
                RoadManager::getInstance()->editorRoad->addRoadFarPointBegin(pos);
            }
            break;
        }
    case A_AddItinerPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add itiner point editorStage: %p, GD: %f, LD: %f, iti: [%s] / [%s], desc: [%s]\n",
                RaceManager::getInstance()->editorStage,
                ItinerManager::getInstance()->editorGlobalDistance,
                ItinerManager::getInstance()->editorLocalDistance,
                ItinerManager::getInstance()->editorItinerImageName.c_str(),
                ItinerManager::getInstance()->editorItinerImageName2.c_str(),
                ItinerManager::getInstance()->editorDescription.c_str());
            if (RaceManager::getInstance()->editorStage)
            {
                ItinerManager::getInstance()->editorGlobalDistance += ItinerManager::getInstance()->editorLocalDistance;
                refreshItinerGD();
                ItinerPoint* ip = new ItinerPoint(apos,
                    ItinerManager::getInstance()->editorGlobalDistance,
                    ItinerManager::getInstance()->editorLocalDistance,
                    ItinerManager::getInstance()->editorItinerImageName,
                    ItinerManager::getInstance()->editorItinerImageName2,
                    ItinerManager::getInstance()->editorDescription);
                RaceManager::getInstance()->editorStage->itinerPointList.push_back(ip);
            }
            break;
        }
    case A_AddAIPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add AI point editorStage: %p\n",
                RaceManager::getInstance()->editorStage);
            if (RaceManager::getInstance()->editorStage)
            {
                float gd = 0.f;
                float ld = 0.f;
                if (!RaceManager::getInstance()->editorStage->AIPointList.empty())
                {
                    irr::core::vector3df lastPos = RaceManager::getInstance()->editorStage->AIPointList.back()->getPos();
                    ld = lastPos.getDistanceFrom(apos);
                    gd = ld + RaceManager::getInstance()->editorStage->AIPointList.back()->getGlobalDistance();
                }
                AIPoint* aip = new AIPoint(apos, gd ,ld);
                RaceManager::getInstance()->editorStage->AIPointList.push_back(aip);
            }
            break;
        }
    case A_AddWayPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): add waypoint editorStage: %p\n",
                RaceManager::getInstance()->editorStage);
            if (RaceManager::getInstance()->editorStage)
            {
                float ld = 10000.f;
                if (!RaceManager::getInstance()->editorStage->wayPointList.empty())
                {
                    irr::core::vector3df lastPos = RaceManager::getInstance()->editorStage->wayPointList.back()->getPos();
                    ld = lastPos.getDistanceFrom(apos);
                }
                
                if (ld > 1500.f)
                {
                    unsigned int num = RaceManager::getInstance()->editorStage->wayPointList.size() + 1;
                    WayPoint* wpip = new WayPoint(apos, num);
                    RaceManager::getInstance()->editorStage->wayPointList.push_back(wpip);
                }
                else
                {
                    dprintf(MY_DEBUG_INFO, "MenuPageEditor::action(): add waypoint not possible, because last WP is to close: %f\n", ld);
                }
            }
            break;
        }
    case A_RemoveObjectRace:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove object race editorRace: %p\n",
                RaceManager::getInstance()->editorRace);
            if (RaceManager::getInstance()->editorRace && !RaceManager::getInstance()->editorRace->globalObjectList.empty())
            {
                ObjectWireGlobalObject* go = RaceManager::getInstance()->editorRace->globalObjectList.back();
                RaceManager::getInstance()->editorRace->globalObjectList.pop_back();
                if (RaceManager::getInstance()->editorRace->active)
                {
                    ObjectWire::getInstance()->removeGlobalObject(go, false);
                }
                delete go;
            }
            break;
        }
    case A_RemoveObjectDay:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove object day editorDay: %p\n",
                RaceManager::getInstance()->editorDay);
            if (RaceManager::getInstance()->editorDay && !RaceManager::getInstance()->editorDay->globalObjectList.empty())
            {
                ObjectWireGlobalObject* go = RaceManager::getInstance()->editorDay->globalObjectList.back();
                RaceManager::getInstance()->editorDay->globalObjectList.pop_back();
                if (RaceManager::getInstance()->editorDay->active)
                {
                    ObjectWire::getInstance()->removeGlobalObject(go, false);
                }
                delete go;
            }
            break;
        }
    case A_RemoveObjectStage:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove object stage editorStage: %p\n",
                RaceManager::getInstance()->editorStage);
            if (RaceManager::getInstance()->editorStage && !RaceManager::getInstance()->editorStage->globalObjectList.empty())
            {
                ObjectWireGlobalObject* go = RaceManager::getInstance()->editorStage->globalObjectList.back();
                RaceManager::getInstance()->editorStage->globalObjectList.pop_back();
                if (RaceManager::getInstance()->editorStage->active)
                {
                    ObjectWire::getInstance()->removeGlobalObject(go, false);
                }
                delete go;
            }
            break;
        }
    case A_RemoveRoadPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove road point, editorRoad: %p\n", RoadManager::getInstance()->editorRoad);
            if (RoadManager::getInstance()->editorRoad)
            {
                RoadManager::getInstance()->editorRoad->removeRoadPoint();
            }
            break;
        }
    case A_RemoveRoadPointBegin:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove road point begin, editorRoad: %p\n", RoadManager::getInstance()->editorRoad);
            if (RoadManager::getInstance()->editorRoad)
            {
                RoadManager::getInstance()->editorRoad->removeRoadPointBegin();
            }
            break;
        }
    case A_RemoveItinerPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove itiner point editorStage: %p\n",
                RaceManager::getInstance()->editorStage);
            if (RaceManager::getInstance()->editorStage && !RaceManager::getInstance()->editorStage->itinerPointList.empty())
            {
                ItinerPoint* ip = RaceManager::getInstance()->editorStage->itinerPointList.back();
                ItinerManager::getInstance()->editorGlobalDistance -= ip->getLocalDistance();
                if (ItinerManager::getInstance()->editorGlobalDistance < 0.f) ItinerManager::getInstance()->editorGlobalDistance = 0.f;
                refreshItinerGD();
                RaceManager::getInstance()->editorStage->itinerPointList.pop_back();
                delete ip;
            }
            break;
        }
    case A_RemoveAIPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove AI point editorStage: %p\n",
                RaceManager::getInstance()->editorStage);
            if (RaceManager::getInstance()->editorStage && !RaceManager::getInstance()->editorStage->AIPointList.empty())
            {
                AIPoint* aip = RaceManager::getInstance()->editorStage->AIPointList.back();
                RaceManager::getInstance()->editorStage->AIPointList.pop_back();
                delete aip;
            }
            break;
        }
    case A_RemoveWayPoint:
        {
            dprintf(MY_DEBUG_NOTE, "MenuPageEditor::action(): remove waypoint editorStage: %p\n",
                RaceManager::getInstance()->editorStage);
            if (RaceManager::getInstance()->editorStage && !RaceManager::getInstance()->editorStage->wayPointList.empty())
            {
                WayPoint* wpip = RaceManager::getInstance()->editorStage->wayPointList.back();
                RaceManager::getInstance()->editorStage->wayPointList.pop_back();
                delete wpip;
            }
            break;
        }
    default:
        dprintf(MY_DEBUG_ERROR, "MenuPageEditor::action(): no current action: %d\n", (int)currentAction);
    }
}

void MenuPageEditor::renderP()
{
    if (/*!checkBoxRender->isChecked()*/!doRender) return;

    TheGame::getInstance()->getDriver()->setMaterial(material);
    TheGame::getInstance()->getDriver()->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);

    if (RaceManager::getInstance()->editorRace)
    {
        RaceManager::editorRenderObjects(RaceManager::getInstance()->editorRace->globalObjectList);
        RoadManager::editorRenderRoads(RaceManager::getInstance()->editorRace->roadMap);
    }
    if (RaceManager::getInstance()->editorDay)
    {
        RaceManager::editorRenderObjects(RaceManager::getInstance()->editorDay->globalObjectList);
        RoadManager::editorRenderRoads(RaceManager::getInstance()->editorDay->roadMap);
    }
    if (RaceManager::getInstance()->editorStage)
    {
        RaceManager::editorRenderObjects(RaceManager::getInstance()->editorStage->globalObjectList);
        ItinerManager::editorRenderItinerPointList(RaceManager::getInstance()->editorStage->itinerPointList);
        AIPoint::editorRenderAIPointList(RaceManager::getInstance()->editorStage->AIPointList);
        WayPointManager::editorRenderWayPointList(RaceManager::getInstance()->editorStage->wayPointList);
        RoadManager::editorRenderRoads(RaceManager::getInstance()->editorStage->roadMap);
    }
    if (RoadManager::getInstance()->editorRoad)
    {
        RoadManager::getInstance()->editorRoad->editorRender(true);
    }
}
