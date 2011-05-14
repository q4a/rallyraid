
#include "MenuPageEditorDay.h"
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

MenuPageEditorDay::MenuPageEditorDay()
    : window(0),
      tableStages(0),
      tableGlobalObjects(0),
      editBoxLongName(0),
      editBoxShortDescription(0),
      editBoxNewStage(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width-350, 50, TheGame::getInstance()->getScreenSize().Width-10, TheGame::getInstance()->getScreenSize().Height-150),
        false,
        L"Editor - Day",
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
        MI_BUTTONCREATESTAGE,
        L"new stage");

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
    // Stages tab
    // ----------------------------
    irr::gui::IGUITab* tabStages = tc->addTab(L"Stages", MI_TABSTAGES);

    editBoxNewStage = TheGame::getInstance()->getEnv()->addEditBox(L"new stage name",
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabStages->getRelativePosition().getSize().Width, 20)),
        true,
        tabStages,
        MI_EBNEWSTAGENAME);


    tableStages = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 22), irr::core::dimension2di(tabStages->getRelativePosition().getSize().Width, tabStages->getRelativePosition().getSize().Height-22)),
        tabStages,
        MI_TABLESTAGES,
        true);

    tableStages->addColumn(L"#");
    tableStages->addColumn(L"name");
    tableStages->addColumn(L"long name");

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

    window->setVisible(false);
}

MenuPageEditorDay::~MenuPageEditorDay()
{
    //window->remove();
    close();
}

bool MenuPageEditorDay::OnEvent(const irr::SEvent &event)
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
                        dprintf(MY_DEBUG_NOTE, "editor::day::save\n");
                        WStringConverter::toString(editBoxLongName->getText(), RaceManager::getInstance()->editorDay->dayLongName);
                        WStringConverter::toString(editBoxShortDescription->getText(), RaceManager::getInstance()->editorDay->shortDescription);
                        RaceManager::getInstance()->editorDay->write();
                        return true;
                        break;
                    case MI_BUTTONCREATESTAGE:
                    {
                        dprintf(MY_DEBUG_NOTE, "editor::day::newStage\n");
                        std::string stage;
                        WStringConverter::toString(editBoxNewStage->getText(), stage);
                        Day::stageMap_t::const_iterator sit = RaceManager::getInstance()->editorDay->stageMap.find(stage);
                        if (sit == RaceManager::getInstance()->editorDay->stageMap.end())
                        {
                            bool ret;
                            ret = ConfigDirectory::mkdir(STAGE_DIR(RaceManager::getInstance()->editorRace->getName(), RaceManager::getInstance()->editorDay->getName(), stage));
                            if (ret)
                            {
                                RaceManager::getInstance()->editorDay->stageMap[stage] = new Stage(RaceManager::getInstance()->editorRace->getName(), RaceManager::getInstance()->editorDay->getName(), stage, ret);
                                refreshStages();
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
                    case MI_TABLESTAGES:
                        RaceManager::getInstance()->editorStage = (Stage*)tableStages->getCellData(tableStages->getSelected(), 0);
                        return true;
                        break;
                };
                break;
            }
            case irr::gui::EGET_TABLE_SELECTED_AGAIN:
            {
                switch (id)
                {
                    case MI_TABLESTAGES:
                        RaceManager::getInstance()->editorStage = (Stage*)tableStages->getCellData(tableStages->getSelected(), 0);
                        MenuManager::getInstance()->open(MenuManager::MP_EDITORSTAGE);
                        return true;
                        break;
                };
                break;
            }
        };
    }
    return false;
}

void MenuPageEditorDay::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditorDay::open()\n");
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(window);
}

void MenuPageEditorDay::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageEditorDay::close()\n");
    window->setVisible(false);
}

void MenuPageEditorDay::refresh()
{
    refreshStages();
    refreshGlobalObjects();
    refreshEditBoxes();
}

void MenuPageEditorDay::refreshStages()
{
    // ----------------------------
    // Stages
    // ----------------------------
    tableStages->clearRows();

    const Day::stageMap_t& stageMap = RaceManager::getInstance()->editorDay->getStageMap();
    unsigned int i = 0;
    for (Day::stageMap_t::const_iterator sit = stageMap.begin();
         sit != stageMap.end();
         sit++, i++)
    {
        irr::core::stringw str;
        
        tableStages->addRow(i);

        str += i;
        tableStages->setCellText(i, 0, str.c_str());
        tableStages->setCellData(i, 0, (void*)sit->second);

        str = L"";
        str += sit->first.c_str();
        tableStages->setCellText(i, 1, str.c_str());

        str = L"";
        str += sit->second->getLongName().c_str();
        tableStages->setCellText(i, 2, str.c_str());
    }
}

void MenuPageEditorDay::refreshGlobalObjects()
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

void MenuPageEditorDay::refreshEditBoxes()
{
    irr::core::stringw str;

    str += RaceManager::getInstance()->editorDay->getLongName().c_str();
    editBoxLongName->setText(str.c_str());

    str = L"";
    str += RaceManager::getInstance()->editorDay->getShortDescription().c_str();
    editBoxShortDescription->setText(str.c_str());

    str = L"";
    editBoxNewStage->setText(str.c_str());
}
