
#include "MenuPageEditorStage.h"
#include "MenuPageEditor.h"
#include "TheGame.h"
#include "stdafx.h"
#include "WStringConverter.h"
#include "ConfigDirectory.h"

#include "TheEarth.h"
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
#include "ObjectWireGlobalObject.h"
#include "Competitor.h"

MenuPageEditorStage::MenuPageEditorStage()
    : window(0),
      tableGlobalObjects(0),
      tableRoads(0),
      editBoxLongName(0),
      editBoxShortDescription(0),
      editBoxNewRoadFilename(0),
      editBoxNewRoadName(0),
      editBoxNewRoadDataFilename(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-350, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
        false,
        L"Editor - Stage",
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
        MI_BUTTONSAVE,
        L"Save");

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(86,22,126,42),
        window,
        MI_BUTTONCREATEROAD,
        L"new road");

    editBoxLongName = TheGame::getInstance()->getEnv()->addEditBox(L"long name",
        irr::core::recti(irr::core::position2di(2, 44), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, 20)),
        true,
        window,
        MI_EBLONGNAME);

    editBoxShortDescription = TheGame::getInstance()->getEnv()->addEditBox(L"short description",
        irr::core::recti(irr::core::position2di(2, 66), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, 20)),
        true,
        window,
        MI_EBSHORTDESCRIPTION);

    irr::gui::IGUITabControl* tc = TheGame::getInstance()->getEnv()->addTabControl(
        irr::core::recti(irr::core::position2di(2, 88), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, window->getRelativePosition().getSize().Height - 90)),
        window,
        true,
        true,
        MI_TABCONTROL);

    // ----------------------------
    // GlobalObject tab
    // ----------------------------
    irr::gui::IGUITab* tabGlobalObjects = tc->addTab(L"GO", MI_TABGLOBALOBJECTS);

    tableGlobalObjects = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabGlobalObjects->getRelativePosition().getSize()),
        tabGlobalObjects,
        MI_TABLEGLOBALOBJECTS,
        true);

    tableGlobalObjects->addColumn(L"#");
    tableGlobalObjects->addColumn(L"object");
    tableGlobalObjects->setColumnWidth(1, 80);
    tableGlobalObjects->addColumn(L"X");
    tableGlobalObjects->addColumn(L"Y");
    tableGlobalObjects->addColumn(L"visible");

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

    window->setVisible(false);
}

MenuPageEditorStage::~MenuPageEditorStage()
{
    //window->remove();
    close();
}

bool MenuPageEditorStage::OnEvent(const irr::SEvent &event)
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
                    case MI_BUTTONSAVE:
                        dprintf(MY_DEBUG_NOTE, "editor::stage::save\n");
                        WStringConverter::toString(editBoxLongName->getText(), RaceManager::getInstance()->editorStage->stageLongName);
                        WStringConverter::toString(editBoxShortDescription->getText(), RaceManager::getInstance()->editorStage->shortDescription);
                        RaceManager::getInstance()->editorStage->write();
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
                        RoadManager::roadMap_t::const_iterator rit = RaceManager::getInstance()->editorStage->roadMap.find(roadName);
                        if (rit == RaceManager::getInstance()->editorStage->roadMap.end() &&
                            !roadName.empty() &&
                            RoadTypeManager::getInstance()->editorRoadType)
                        {
                            bool ret;
                            ret = ConfigDirectory::mkdir(STAGE_ROADS(RaceManager::getInstance()->editorRace->getName(),RaceManager::getInstance()->editorDay->getName(),RaceManager::getInstance()->editorStage->getName())) &&
                                ConfigDirectory::mkdir(STAGE_ROADS_DATA(RaceManager::getInstance()->editorRace->getName(),RaceManager::getInstance()->editorDay->getName(),RaceManager::getInstance()->editorStage->getName()));
                            if (ret)
                            {
                                RaceManager::getInstance()->editorStage->roadMap[roadName] =
                                    new Road(roadFilename, roadName, roadDataFilename, RoadTypeManager::getInstance()->editorRoadType, false);
                                refreshRoads();
                            }
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
        };
    }
    return false;
}

void MenuPageEditorStage::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditorStage::open()\n");
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(window);
}

void MenuPageEditorStage::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditorStage::close()\n");
    window->setVisible(false);
}

void MenuPageEditorStage::refresh()
{
    refreshGlobalObjects();
    refreshEditBoxes();
    refreshRoads();
}

void MenuPageEditorStage::refreshGlobalObjects()
{
    // ----------------------------
    // GlobalObjects
    // ----------------------------
    tableGlobalObjects->clearRows();
    /*
    tableGlobalObjects->addColumn(L"#");
    tableGlobalObjects->addColumn(L"object");
    tableGlobalObjects->addColumn(L"X");
    tableGlobalObjects->addColumn(L"Y");
    tableGlobalObjects->addColumn(L"visible");
    */
    const RaceManager::globalObjectList_t& globalObjectList = RaceManager::getInstance()->editorStage->globalObjectList;
    unsigned int i = 0;
    for (RaceManager::globalObjectList_t::const_iterator goit = globalObjectList.begin();
         goit != globalObjectList.end();
         goit++, i++)
    {
        irr::core::stringw str;
        
        tableGlobalObjects->addRow(i);

        str += i;
        tableGlobalObjects->setCellText(i, 0, str.c_str());

        str = L"";
        str += (*goit)->getObjectPool()->getName().c_str();
        tableGlobalObjects->setCellText(i, 1, str.c_str());

        str = L"";
        str += (*goit)->getPos().X;
        tableGlobalObjects->setCellText(i, 2, str.c_str());

        str = L"";
        str += (*goit)->getPos().Z;
        tableGlobalObjects->setCellText(i, 3, str.c_str());

        str = L"false";
        if ((*goit)->getVisible()) str = L"true";
        tableGlobalObjects->setCellText(i, 4, str.c_str());

    }
}

void MenuPageEditorStage::refreshEditBoxes()
{
    irr::core::stringw str;

    str += RaceManager::getInstance()->editorStage->getLongName().c_str();
    editBoxLongName->setText(str.c_str());

    str = L"";
    str += RaceManager::getInstance()->editorStage->getShortDescription().c_str();
    editBoxShortDescription->setText(str.c_str());
}

void MenuPageEditorStage::refreshRoads()
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
    const RoadManager::roadMap_t& roadMap = RaceManager::getInstance()->editorStage->getRoadMap();
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

}

void MenuPageEditorStage::refreshRoadEditBoxes(const wchar_t* newRoadName)
{
    irr::core::stringw str;

    str = newRoadName;
    editBoxNewRoadName->setText(str.c_str());

    str = L"";
    str += (STAGE_ROADS(RaceManager::getInstance()->editorRace->getName(),
        RaceManager::getInstance()->editorDay->getName(),
        RaceManager::getInstance()->editorStage->getName())+std::string("/")).c_str();
    str += newRoadName;
    editBoxNewRoadFilename->setText(str.c_str());

    str = L"";
    str += (STAGE_ROADS_DATA(RaceManager::getInstance()->editorRace->getName(),
        RaceManager::getInstance()->editorDay->getName(),
        RaceManager::getInstance()->editorStage->getName())+std::string("/")).c_str();
    str += newRoadName;
    editBoxNewRoadDataFilename->setText(str.c_str());

}
