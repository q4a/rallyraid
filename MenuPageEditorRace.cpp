
#include "MenuPageEditorRace.h"
#include "TheGame.h"
#include "stdafx.h"
#include "WStringConverter.h"

#include "TheEarth.h"
#include "ObjectPool.h"
#include "ObjectPoolManager.h"
#include "RaceManager.h"
#include "Race.h"
#include "Settings.h"
#include "RoadManager.h"
#include "Road.h"
#include "Day.h"
#include "Stage.h"
#include "RoadType.h"
#include "ObjectWireGlobalObject.h"
#include "Competitor.h"

MenuPageEditorRace::MenuPageEditorRace()
    : window(0),
      tableDays(0),
      tableCompetitors(0),
      tableGlobalObjects(0),
      editBoxLongName(0),
      editBoxShortDescription(0),
      editBoxNewDay(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-300, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
        false,
        L"Editor - Race",
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
        MI_BUTTONCREATEDAY,
        L"new day");

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
    // Days tab
    // ----------------------------
    irr::gui::IGUITab* tabDays = tc->addTab(L"Days", MI_TABDAYS);

    editBoxNewDay = TheGame::getInstance()->getEnv()->addEditBox(L"new day name",
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabDays->getRelativePosition().getSize().Width, 20)),
        true,
        tabDays,
        MI_EBNEWDAYNAME);


    tableDays = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 22), irr::core::dimension2di(tabDays->getRelativePosition().getSize().Width, tabDays->getRelativePosition().getSize().Height-22)),
        tabDays,
        MI_TABLEDAYS,
        true);

    tableDays->addColumn(L"#");
    tableDays->addColumn(L"name");
    tableDays->addColumn(L"long name");

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
    tableGlobalObjects->addColumn(L"X");
    tableGlobalObjects->addColumn(L"Y");
    tableGlobalObjects->addColumn(L"visible");

    // ----------------------------
    // Competitors tab
    // ----------------------------
    irr::gui::IGUITab* tabCompetitors = tc->addTab(L"Comp", MI_TABCOMPETITORS);

    tableCompetitors = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabGlobalObjects->getRelativePosition().getSize()),
        tabCompetitors,
        MI_TABLECOMPETITORS,
        true);

    tableCompetitors->addColumn(L"#");
    tableCompetitors->addColumn(L"num");
    tableCompetitors->addColumn(L"name");
    tableCompetitors->addColumn(L"co-name");
    tableCompetitors->addColumn(L"team");
    tableCompetitors->addColumn(L"vehicle");
    tableCompetitors->addColumn(L"strength");

    window->setVisible(false);
}

MenuPageEditorRace::~MenuPageEditorRace()
{
    //window->remove();
    close();
}

bool MenuPageEditorRace::OnEvent(const irr::SEvent &event)
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
                        dprintf(MY_DEBUG_NOTE, "editor::race::save\n");
                        WStringConverter::toString(editBoxLongName->getText(), RaceManager::getInstance()->editorRace->raceLongName);
                        WStringConverter::toString(editBoxShortDescription->getText(), RaceManager::getInstance()->editorRace->shortDescription);
                        RaceManager::getInstance()->editorRace->write();
                        return true;
                        break;
                };
                break;
            }
            case irr::gui::EGET_TABLE_CHANGED:
            {
                switch (id)
                {
                    case MI_TABLEDAYS:
                        RaceManager::getInstance()->editorDay = (Day*)tableDays->getCellData(tableDays->getSelected(), 0);
                        return true;
                        break;
                };
                break;
            }
        };
    }
    return false;
}

void MenuPageEditorRace::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::open()\n");
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(window);
}

void MenuPageEditorRace::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditor::close()\n");
    window->setVisible(false);
}

void MenuPageEditorRace::refresh()
{
    refreshDays();
    refreshGlobalObjects();
    refreshCompetitors();
    refreshEditBoxes();
}

void MenuPageEditorRace::refreshDays()
{
    // ----------------------------
    // Days
    // ----------------------------
    tableDays->clearRows();

    const Race::dayMap_t& dayMap = RaceManager::getInstance()->editorRace->getDayMap();
    unsigned int i = 0;
    for (Race::dayMap_t::const_iterator dit = dayMap.begin();
         dit != dayMap.end();
         dit++, i++)
    {
        irr::core::stringw str;
        
        tableDays->addRow(i);

        str += i;
        tableDays->setCellText(i, 0, str.c_str());
        tableDays->setCellData(i, 0, (void*)dit->second);

        str = L"";
        str += dit->first.c_str();
        tableDays->setCellText(i, 1, str.c_str());

        str = L"";
        str += dit->second->getLongName().c_str();
        tableDays->setCellText(i, 2, str.c_str());
    }
}

void MenuPageEditorRace::refreshGlobalObjects()
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
    const RaceManager::globalObjectList_t& globalObjectList = RaceManager::getInstance()->editorRace->globalObjectList;
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

void MenuPageEditorRace::refreshCompetitors()
{
    // ----------------------------
    // Competitors
    // ----------------------------
    tableCompetitors->clearRows();
    /*
    tableCompetitors->addColumn(L"#");
    tableCompetitors->addColumn(L"num");
    tableCompetitors->addColumn(L"name");
    tableCompetitors->addColumn(L"co-name");
    tableCompetitors->addColumn(L"team");
    tableCompetitors->addColumn(L"vehicle");
    tableCompetitors->addColumn(L"strength");
    */
    const Race::competitorMap_t& competitorMap = RaceManager::getInstance()->editorRace->getCompetitorMap();
    unsigned int i = 0;
    for (Race::competitorMap_t::const_iterator cit = competitorMap.begin();
         cit != competitorMap.end();
         cit++, i++)
    {
        irr::core::stringw str;
        
        tableCompetitors->addRow(i);

        str += i;
        tableCompetitors->setCellText(i, 0, str.c_str());

        str = L"";
        str += cit->second->getNum();
        tableCompetitors->setCellText(i, 1, str.c_str());

        str = L"";
        str += cit->second->getName().c_str();
        tableCompetitors->setCellText(i, 2, str.c_str());

        str = L"";
        str += cit->second->getCoName().c_str();
        tableCompetitors->setCellText(i, 3, str.c_str());

        str = L"";
        str += cit->second->getTeamName().c_str();
        tableCompetitors->setCellText(i, 4, str.c_str());

        str = L"";
        str += cit->second->getVehicleTypeName().c_str();
        tableCompetitors->setCellText(i, 5, str.c_str());

        str = L"";
        str += cit->second->getStrength();
        tableCompetitors->setCellText(i, 6, str.c_str());
    }
}

void MenuPageEditorRace::refreshEditBoxes()
{
    irr::core::stringw str;

    str += RaceManager::getInstance()->editorRace->getLongName().c_str();
    editBoxLongName->setText(str.c_str());

    str = L"";
    str += RaceManager::getInstance()->editorRace->getShortDescription().c_str();
    editBoxShortDescription->setText(str.c_str());

    str = L"";
    //str += DAY_DIR(RaceManager::getInstance()->editorRace->getName(), "").c_str();
    editBoxNewDay->setText(str.c_str());
}
