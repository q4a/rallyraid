
#ifndef MENUPAGEEDITOR_H
#define MENUPAGEEDITOR_H

#include "MenuPageBase.h"
#include "MenuManager.h"

class MenuPageEditor : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_EDITOR,
        NUMBER_OF_MENUITEMS
    };


    MenuPageEditor();
    virtual ~MenuPageEditor();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    irr::gui::IGUIWindow*   window;
};

#endif // MENUPAGEEDITOR_H
