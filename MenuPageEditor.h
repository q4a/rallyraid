
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
        MI_TABOBJECTWIRE,
        MI_TABOBJECTPOOL,
        MI_TABRACEMANAGER,
        MI_TAB5,
        MI_TABLETILES,
        MI_TABLEOBJECTWIRE,
        MI_TABLEOBJECTPOOL,
        MI_TABLERACEMANAGER,
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
    irr::gui::IGUITable*    tableObjectWire;
    irr::gui::IGUITable*    tableObjectPool;
    irr::gui::IGUITable*    tableRaceManager;
};

#endif // MENUPAGEEDITOR_H
