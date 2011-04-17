
#ifndef MENUPAGEEDITORDAY_H
#define MENUPAGEEDITORDAY_H

#include "MenuPageBase.h"
#include "MenuManager.h"

class MenuPageEditorDay : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_EDITORDAY,
        MI_BUTTONREFRESH,
        MI_BUTTONSAVE,
        MI_BUTTONCREATESTAGE,
        MI_TABCONTROL,

        MI_TABSTAGES,
        MI_TABGLOBALOBJECTS,

        MI_TABLESTAGES,
        MI_TABLEGLOBALOBJECTS,

        MI_EBLONGNAME,
        MI_EBSHORTDESCRIPTION,
        MI_EBNEWSTAGENAME,

        NUMBER_OF_MENUITEMS
    };


    MenuPageEditorDay();
    virtual ~MenuPageEditorDay();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshStages();
    void refreshGlobalObjects();
    void refreshEditBoxes();

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableStages;
    irr::gui::IGUITable*    tableGlobalObjects;
    irr::gui::IGUIEditBox*  editBoxLongName;
    irr::gui::IGUIEditBox*  editBoxShortDescription;
    irr::gui::IGUIEditBox*  editBoxNewStage;
};

#endif // MENUPAGEEDITORDAY_H
