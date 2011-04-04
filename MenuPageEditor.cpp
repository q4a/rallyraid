
#include "MenuPageEditor.h"
#include "TheGame.h"
#include "stdafx.h"

MenuPageEditor::MenuPageEditor()
    : window(0)
{
    window = TheGame::getInstance()->getEnv()->addWindow(
        irr::core::recti(10, 50, 400, 400), false, L"Editor", 0, MI_WINDOW);
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
        switch (event.GUIEvent.EventType)
        {
            case irr::gui::EGET_ELEMENT_CLOSED:
            {
                switch (event.GUIEvent.Caller->getID())
                {
                    case MI_WINDOW:
                        dprintf(MY_DEBUG_NOTE, "event on close editor window\n");
                        MenuManager::getInstance()->close();
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
