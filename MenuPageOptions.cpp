
#include "MenuPageOptions.h"
#include "MenuPageOptionsKB.h"
#include "MenuPageStage.h"
#include "TheGame.h"
#include "stdafx.h"
#include "WStringConverter.h"
#include "ConfigDirectory.h"
#include "EventReceiver.h"
#include "Settings.h"
#include "KeyConfig.h"
#include <assert.h>


MenuPageOptions* MenuPageOptions::menuPageOptions = 0;

MenuPageOptions::MenuPageOptions()
    : window(0),
      tableKB(0),
      lastKeyName(0),
      primary(true)
{
    menuPageOptions = this;
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(TheGame::getInstance()->getScreenSize().Width/2-350, TheGame::getInstance()->getScreenSize().Height/2-300, TheGame::getInstance()->getScreenSize().Width/2+350, TheGame::getInstance()->getScreenSize().Height/2+300),
        false,
        L"Options",
        0,
        MI_WINDOW);

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(irr::core::position2di(window->getRelativePosition().getSize().Width-82, window->getRelativePosition().getSize().Height-22), irr::core::dimension2du(80, 20)),
        window,
        MI_BUTTONSAVE,
        L"Save Settings");

    irr::gui::IGUITabControl* tc = TheGame::getInstance()->getEnv()->addTabControl(
        irr::core::recti(irr::core::position2di(2, 22), irr::core::dimension2di(window->getRelativePosition().getSize().Width - 4, window->getRelativePosition().getSize().Height - 46)),
        window,
        true,
        true,
        0);

    // ----------------------------
    // General
    // ----------------------------
    irr::gui::IGUITab* tabGeneral = tc->addTab(L"General", 0);


    // ----------------------------
    // Input
    // ----------------------------
    irr::gui::IGUITab* tabKB = tc->addTab(L"Input", 0);

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(irr::core::position2di(0, 0), irr::core::dimension2di(tabKB->getRelativePosition().getSize().Width/3-2, 20)),
        tabKB,
        MI_BUTTONCENTER,
        L"Recalibrate Joystick");

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(irr::core::position2di(tabKB->getRelativePosition().getSize().Width/3, 0), irr::core::dimension2di(tabKB->getRelativePosition().getSize().Width/3-2, 20)),
        tabKB,
        MI_BUTTONPRIMARY,
        L"Set Primary");

    TheGame::getInstance()->getEnv()->addButton(
        irr::core::recti(irr::core::position2di(tabKB->getRelativePosition().getSize().Width*2/3, 0), irr::core::dimension2di(tabKB->getRelativePosition().getSize().Width/3-2, 20)),
        tabKB,
        MI_BUTTONSECONDARY,
        L"Set Secondary");

    tableKB = TheGame::getInstance()->getEnv()->addTable(
        irr::core::recti(irr::core::position2di(0, 22), irr::core::dimension2di(tabKB->getRelativePosition().getSize().Width, tabKB->getRelativePosition().getSize().Height-22)),
        tabKB,
        MI_TABLEKB,
        true);

    tableKB->addColumn(L"Action");
    tableKB->setColumnWidth(0, (tableKB->getRelativePosition().getSize().Width-16)/3);
    tableKB->addColumn(L"Primary");
    tableKB->setColumnWidth(1, (tableKB->getRelativePosition().getSize().Width-16)/3);
    tableKB->addColumn(L"Secondary");
    tableKB->setColumnWidth(2, (tableKB->getRelativePosition().getSize().Width-16)/3);


    window->setVisible(false);
}

MenuPageOptions::~MenuPageOptions()
{
    //window->remove();
    menuPageOptions = 0;
    close();
}

bool MenuPageOptions::OnEvent(const irr::SEvent &event)
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
                    case MI_BUTTONSAVE:
                        dprintf(MY_DEBUG_NOTE, "ingamemenu::savebutton::clicked\n");
                        Settings::getInstance()->write();
                        TheGame::getInstance()->getEventReceiver()->saveKeyMapping();
                        //MenuManager::getInstance()->close();
                        return true;
                        break;
                    case MI_BUTTONCENTER:
                        dprintf(MY_DEBUG_NOTE, "ingamemenu::recalibratebutton::clicked\n");
                        TheGame::getInstance()->getEventReceiver()->recalibrate();
                        //MenuManager::getInstance()->close();
                        return true;
                        break;
                    case MI_BUTTONPRIMARY:
                        dprintf(MY_DEBUG_NOTE, "ingamemenu::primarybutton::clicked\n");
                        primary = true;
                        lastKeyName = (int)tableKB->getCellData(tableKB->getSelected(), 0);
                        MenuManager::getInstance()->open(MenuManager::MP_OPTIONSKB);
                        return true;
                        break;
                    case MI_BUTTONSECONDARY:
                        dprintf(MY_DEBUG_NOTE, "ingamemenu::secondarybutton::clicked\n");
                        primary = false;
                        lastKeyName = (int)tableKB->getCellData(tableKB->getSelected(), 0);
                        MenuManager::getInstance()->open(MenuManager::MP_OPTIONSKB);
                        return true;
                        break;
                };
                break;
            }
            
            case irr::gui::EGET_TABLE_CHANGED:
            {
                switch (id)
                {
                    case MI_TABLEKB:
                        return true;
                        break;
                };
                break;
            }
        };
    }
    return false;
}

void MenuPageOptions::open()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageOptions::open()\n");
    refresh();
    window->setVisible(true);
    TheGame::getInstance()->getEnv()->setFocus(window);
}

void MenuPageOptions::close()
{
    dprintf(MY_DEBUG_NOTE, "MenuPageOptions::close()\n");
    window->setVisible(false);
}

void MenuPageOptions::refresh()
{
    refreshGeneral();
    refreshKB();
}

void MenuPageOptions::refreshGeneral()
{
}

void MenuPageOptions::refreshKB()
{
    // ----------------------------
    // Input - KB
    // ----------------------------
    tableKB->clearRows();

    EventReceiver* er = TheGame::getInstance()->getEventReceiver();
    const EventReceiver::keyNameMap_t& keyNameMap = er->keyNameMap;
    unsigned int i = 0;
    for (EventReceiver::keyNameMap_t::const_iterator it = keyNameMap.begin();
         it != keyNameMap.end();
         it++, i++)
    {
        irr::core::stringw str;
        
        tableKB->addRow(i);

        str = L"";
        str += it->first.c_str();
        tableKB->setCellText(i, 0, str.c_str());
        tableKB->setCellData(i, 0, (void*)(it->second));

        str = L"";
        if (er->keyMap[it->second].primaryKeyConfig)
        {
            str += er->keyMap[it->second].primaryKeyConfig->getName();
        }
        else
        {
            str += L"<empty>";
        }
        tableKB->setCellText(i, 1, str.c_str());

        str = L"";
        if (er->keyMap[it->second].secondaryKeyConfig)
        {
            str += er->keyMap[it->second].secondaryKeyConfig->getName();
        }
        else
        {
            str += L"<empty>";
        }
        tableKB->setCellText(i, 2, str.c_str());
    }
}

void MenuPageOptions::optionKBClosed()
{
    EventReceiver* er = TheGame::getInstance()->getEventReceiver();

    if (primary)
    {
        if (er->keyMap[lastKeyName].primaryKeyConfig)
        {
            delete er->keyMap[lastKeyName].primaryKeyConfig;
        }
        er->keyMap[lastKeyName].primaryKeyConfig = MenuPageOptionsKB::menuPageOptionsKB->kc;
    }
    else
    {
        if (er->keyMap[lastKeyName].secondaryKeyConfig)
        {
            delete er->keyMap[lastKeyName].secondaryKeyConfig;
        }
        er->keyMap[lastKeyName].secondaryKeyConfig = MenuPageOptionsKB::menuPageOptionsKB->kc;
    }
    refreshKB();
}
