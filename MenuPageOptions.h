
#ifndef MENUPAGEOPTIONS_H
#define MENUPAGEOPTIONS_H

#include "MenuPageBase.h"
#include "MenuManager.h"


class StageState;

class MenuPageOptions : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_OPTIONS,
        MI_BUTTONSAVE,
        MI_BUTTONCENTER,
        MI_BUTTONPRIMARY,
        MI_BUTTONSECONDARY,

        MI_TABLEKB,

        NUMBER_OF_MENUITEMS
    };


    MenuPageOptions();
    virtual ~MenuPageOptions();

    static MenuPageOptions* menuPageOptions;

public:
    void optionKBClosed();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshGeneral();
    void refreshKB();

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableKB;

    int                     lastKeyName;
    bool                    primary;
};

#endif // MENUPAGEOPTIONS_H
