
#ifndef MENUPAGEEDITORRACE_H
#define MENUPAGEEDITORRACE_H

#include "MenuPageBase.h"
#include "MenuManager.h"

class MenuPageEditorRace : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_EDITORRACE,
        MI_BUTTONREFRESH,
        MI_BUTTONSAVE,
        MI_BUTTONCREATEDAY,
        MI_TABCONTROL,

        MI_TABDAYS,
        MI_TABCOMPETITORS,
        MI_TABGLOBALOBJECTS,

        MI_TABLEDAYS,
        MI_TABLECOMPETITORS,
        MI_TABLEGLOBALOBJECTS,

        MI_EBLONGNAME,
        MI_EBSHORTDESCRIPTION,
        MI_EBNEWDAYNAME,

        NUMBER_OF_MENUITEMS
    };


    MenuPageEditorRace();
    virtual ~MenuPageEditorRace();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshDays();
    void refreshCompetitors();
    void refreshGlobalObjects();
    void refreshEditBoxes();

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableDays;
    irr::gui::IGUITable*    tableCompetitors;
    irr::gui::IGUITable*    tableGlobalObjects;
    irr::gui::IGUIEditBox*  editBoxLongName;
    irr::gui::IGUIEditBox*  editBoxShortDescription;
    irr::gui::IGUIEditBox*  editBoxNewDay;
};

#endif // MENUPAGEEDITORRACE_H
