
#ifndef MENUPAGEEDITORROAD_H
#define MENUPAGEEDITORROAD_H

#include "MenuPageBase.h"
#include "MenuManager.h"

class MenuPageEditorRoad : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_EDITORROAD,
        MI_BUTTONREFRESH,
        MI_BUTTONSAVE,
        MI_TABCONTROL,

        MI_TABPOINTS,

        MI_TABLEPOINTS,

        NUMBER_OF_MENUITEMS
    };


    MenuPageEditorRoad();
    virtual ~MenuPageEditorRoad();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshPoints();

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tablePoints;
};

#endif // MENUPAGEEDITORROAD_H
