
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
        MI_BUTTONREFRESH,
        MI_TABCONTROL,
        MI_TABTILES,
        MI_TAB2,
        MI_TAB3,
        MI_TAB4,
        MI_TAB5,
        MI_TABLETILES,
        NUMBER_OF_MENUITEMS
    };


    MenuPageEditor();
    virtual ~MenuPageEditor();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableTiles;
};

#endif // MENUPAGEEDITOR_H
