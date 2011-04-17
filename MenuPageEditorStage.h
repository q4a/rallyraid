
#ifndef MENUPAGEEDITORSTAGE_H
#define MENUPAGEEDITORSTAGE_H

#include "MenuPageBase.h"
#include "MenuManager.h"

class MenuPageEditorStage : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_EDITORSTAGE,
        MI_BUTTONREFRESH,
        MI_BUTTONSAVE,
        MI_TABCONTROL,

        MI_TABGLOBALOBJECTS,

        MI_TABLEGLOBALOBJECTS,

        MI_EBLONGNAME,
        MI_EBSHORTDESCRIPTION,

        NUMBER_OF_MENUITEMS
    };


    MenuPageEditorStage();
    virtual ~MenuPageEditorStage();

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshGlobalObjects();
    void refreshEditBoxes();

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableGlobalObjects;
    irr::gui::IGUIEditBox*  editBoxLongName;
    irr::gui::IGUIEditBox*  editBoxShortDescription;
};

#endif // MENUPAGEEDITORSTAGE_H
