
#include "MenuPageInGame.h"
#include "MenuPageStage.h"
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
#include "VehicleTypeManager.h"
#include "VehicleType.h"
#include "GamePlay.h"
#include "Hud.h"
#include "VehicleManager.h"
#include <assert.h>


MenuPageInGame* MenuPageInGame::menuPageInGame = 0;

MenuPageInGame::MenuPageInGame()
    : window(0),
      tableStages(0),
      tableCompetitors(0)
{
    menuPageInGame = this;
    window = TheGame::getInstance()->getEnv()->addImage(
        irr::core::recti(0, 0, TheGame::getInstance()->getScreenSize().Width, TheGame::getInstance()->getScreenSize().Height),
        0,
        MI_WINDOW);
    window->setScaleImage(true);
    window->setImage(TheGame::getInstance()->getDriver()->getTexture("data/bg/3.jpg"));

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(10,60,90,80),
        window,
        MI_BUTTONBACK,
        L"Back To Game");


    // ----------------------------
    // Stages
    // ----------------------------
    tableStages = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(window->getRelativePosition().getSize().Width/4, (window->getRelativePosition().getSize().Height*2)/3), irr::core::dimension2di(window->getRelativePosition().getSize().Width/4-2,(window->getRelativePosition().getSize().Height)/3-2)),
        window,
        MI_TABLESTAGES,
        true);

    tableStages->addColumn(L"Stage");
    tableStages->setColumnWidth(0, tableStages->getRelativePosition().getSize().Width-16);

    // ----------------------------
    // Competitors
    // ----------------------------
    irr::gui::IGUITabControl* tc = TheGame::getInstance()->getEnv()->addTabControl(
        irr::core::recti(irr::core::position2di(window->getRelativePosition().getSize().Width/2+2, (window->getRelativePosition().getSize().Height*2)/3), irr::core::dimension2di(window->getRelativePosition().getSize().Width/4-2,window->getRelativePosition().getSize().Height/3-2)),
        window,
        true,
        true,
        0);

    irr::gui::IGUITab* tabStage = tc->addTab(L"Stage", 0);

    tableCompetitors = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabStage->getRelativePosition().getSize()),
        tabStage,
        MI_TABLECOMPETITORS,
        true);

    tableCompetitors->addColumn(L"Pos");
    tableCompetitors->addColumn(L"Time");
    tableCompetitors->addColumn(L"Num");
    tableCompetitors->addColumn(L"Name");
    //tableCompetitors->setColumnWidth(0, tableCompetitors->getRelativePosition().getSize().Width-16);

   irr::gui::IGUITab* tabGlobal = tc->addTab(L"Overall", 0);

    tableCompetitorsG = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 0), tabGlobal->getRelativePosition().getSize()),
        tabGlobal,
        MI_TABLECOMPETITORSG,
        true);

    tableCompetitorsG->addColumn(L"Pos");
    tableCompetitorsG->addColumn(L"Time");
    tableCompetitorsG->addColumn(L"Num");
    tableCompetitorsG->addColumn(L"Name");

    window->setVisible(false);
}

MenuPageInGame::~MenuPageInGame()
{
    //window->remove();
    menuPageInGame = 0;
    close();
}

bool MenuPageInGame::OnEvent(const irr::SEvent &event)
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
                    case MI_BUTTONBACK:
                        dprintf(MY_DEBUG_NOTE, "ingamemenu::backbutton::clicked\n");
                        MenuManager::getInstance()->close();
                        return true;
                        break;
                };
                break;
            }
            
            case irr::gui::EGET_TABLE_CHANGED:
            {
                switch (id)
                {
                    case MI_TABLESTAGES:
                        refreshCompetitors((StageState*)tableStages->getCellData(tableStages->getSelected(), 0));
                        return true;
                        break;
                };
                break;
            }
        };
    }
    return false;
}

void MenuPageInGame::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageInGame::open()\n");
    refresh();
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(tableStages);
    
    // stop necessarry elements in the game
    VehicleManager::getInstance()->pause();
    TheGame::getInstance()->setInGame(false);
    Hud::getInstance()->setVisible(false);
}

void MenuPageInGame::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageInGame::close()\n");
    window->setVisible(false);

    // start necessarry elements in the game if returns
    TheGame::getInstance()->setInGame(true);
    Hud::getInstance()->setVisible(true);
    VehicleManager::getInstance()->resume();
}

void MenuPageInGame::refresh()
{
    assert(!GamePlay::getInstance()->raceState.empty());
    refreshStages();
    refreshCompetitors(GamePlay::getInstance()->raceState.back());
}

void MenuPageInGame::refreshStages()
{
    // ----------------------------
    // Stages
    // ----------------------------
    tableStages->clearRows();

    const stageStateList_t& stageStateList = GamePlay::getInstance()->raceState;
    unsigned int i = 0;
    for (stageStateList_t::const_iterator it = stageStateList.begin();
         it != stageStateList.end();
         it++, i++)
    {
        irr::core::stringw str;
        
        tableStages->addRow(i);

        str = L"";
        str += (*it)->stage->getLongName().c_str();
        tableStages->setCellText(i, 0, str.c_str());
        tableStages->setCellData(i, 0, (void*)(*it));
    }
}

void MenuPageInGame::refreshCompetitors(StageState* stageState)
{
    /*
    tableCompetitors->addColumn(L"Pos");
    tableCompetitors->addColumn(L"Time");
    tableCompetitors->addColumn(L"Num");
    tableCompetitors->addColumn(L"Name");
    */
    // ----------------------------
    // Stage result
    // ----------------------------
    tableCompetitors->clearRows();

    const competitorResultList_t& competitorResultList = stageState->competitorResultListStage;
    unsigned int i = 0;
    for (competitorResultList_t::const_iterator it = competitorResultList.begin();
         it != competitorResultList.end();
         it++, i++)
    {
        irr::core::stringw str;
        
        tableCompetitors->addRow(i);

        str = L"";
        str += (i+1);
        tableStages->setCellText(i, 0, str.c_str());
        //tableStages->setCellData(i, 0, (void*)(*it));

        str = L"";
        WStringConverter::addTimeToStr(str, (*it)->stageTime + (*it)->stagePenalityTime);
        tableStages->setCellText(i, 1, str.c_str());

        str = L"";
        str += (*it)->competitor->getNum();
        tableStages->setCellText(i, 2, str.c_str());

        str = L"";
        str += (*it)->competitor->getName().c_str();
        tableStages->setCellText(i, 3, str.c_str());
    }
}
