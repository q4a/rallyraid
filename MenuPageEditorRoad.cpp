
#include "MenuPageEditorRoad.h"
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
#include "Road.h"
#include "Day.h"
#include "Stage.h"
#include "RoadType.h"
#include "ObjectWireGlobalObject.h"
#include "Competitor.h"

MenuPageEditorRoad::MenuPageEditorRoad()
    : window(0),
      tablePoints(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-350, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
        false,
        L"Editor - Road",
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

    irr::gui::IGUITabControl* tc = TheGame::getInstance()->getEnv()->addTabControl(
        irr::core::recti(irr::core::position2di(2, 44), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, window->getRelativePosition().getSize().Height - 46)),
        window,
        true,
        true,
        MI_TABCONTROL);

    // ----------------------------
    // Points tab
    // ----------------------------
    irr::gui::IGUITab* tabPoints = tc->addTab(L"Points", MI_TABPOINTS);

    tablePoints = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabPoints->getRelativePosition().getSize().Width, tabPoints->getRelativePosition().getSize().Height-0)),
        tabPoints,
        MI_TABLEPOINTS,
        true);

    tablePoints->addColumn(L"#");
    tablePoints->addColumn(L"X");
    tablePoints->addColumn(L"Y");
    tablePoints->addColumn(L"Z");

    window->setVisible(false);
}

MenuPageEditorRoad::~MenuPageEditorRoad()
{
    //window->remove();
    close();
}

bool MenuPageEditorRoad::OnEvent(const irr::SEvent &event)
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
                        dprintf(MY_DEBUG_NOTE, "editor::road::save\n");
                        //WStringConverter::toString(editBoxLongName->getText(), RaceManager::getInstance()->editorRace->raceLongName);
                        //WStringConverter::toString(editBoxShortDescription->getText(), RaceManager::getInstance()->editorRace->shortDescription);
                        RoadManager::getInstance()->editorRoad->writeHeader();
                        RoadManager::getInstance()->editorRoad->writeData();
                        return true;
                        break;
                };
                break;
            }
            /*
            case irr::gui::EGET_TABLE_CHANGED:
            {
                switch (id)
                {
                    case MI_TABLEPOINTS:
                        RaceManager::getInstance()->editorDay = (Day*)tableDays->getCellData(tableDays->getSelected(), 0);
                        return true;
                        break;
                };
                break;
            }
            
            case irr::gui::EGET_TABLE_SELECTED_AGAIN:
            {
                switch (id)
                {
                    case MI_TABLEDAYS:
                        RaceManager::getInstance()->editorDay = (Day*)tableDays->getCellData(tableDays->getSelected(), 0);
                        MenuManager::getInstance()->open(MenuManager::MP_EDITORDAY);
                        return true;
                        break;
                };
                break;
            }
            */
        };
    }
    return false;
}

void MenuPageEditorRoad::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditorRoad::open()\n");
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(window);
}

void MenuPageEditorRoad::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditorRoad::close()\n");
    window->setVisible(false);
}

void MenuPageEditorRoad::refresh()
{
    refreshPoints();
}

void MenuPageEditorRoad::refreshPoints()
{
    // ----------------------------
    // Points
    // ----------------------------
    tablePoints->clearRows();

    const Road::roadPointVector_t& roadPointVector = RoadManager::getInstance()->editorRoad->roadPointVector;
    unsigned int i = 0;
    for (Road::roadPointVector_t::const_iterator rpit = roadPointVector.begin();
         rpit != roadPointVector.end();
         rpit++, i++)
    {
        irr::core::stringw str;
        
        tablePoints->addRow(i);

        str += i;
        tablePoints->setCellText(i, 0, str.c_str());
        //tablePoints->setCellData(i, 0, (void*)rpit->second);

        str = L"";
        str += rpit->X;
        tablePoints->setCellText(i, 1, str.c_str());

        str = L"";
        str += rpit->Y;
        tablePoints->setCellText(i, 2, str.c_str());

        str = L"";
        str += rpit->Z;
        tablePoints->setCellText(i, 3, str.c_str());
    }
}
