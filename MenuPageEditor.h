
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
        MI_BUTTONCREATEROAD,
        MI_TABCONTROL,

        MI_TABSELECTED,
        MI_TABTILES,
        MI_TABOBJECTWIRE,
        MI_TABOBJECTWIRETILES,
        MI_TABOBJECTPOOL,
        MI_TABRACEMANAGER,
        MI_TABROADMANAGER,
        MI_TABROADS,

        MI_TABLESELECTED,
        MI_TABLETILES,
        MI_TABLEOBJECTWIRE,
        MI_TABLEOBJECTWIRETILES,
        MI_TABLEOBJECTPOOL,
        MI_TABLERACEMANAGER,
        MI_TABLEROADMANAGERG,
        MI_TABLEROADMANAGERS,
        MI_TABLEROADMANAGERV,
        MI_TABLEROADTYPES,
        MI_TABLEROADS,

        MI_EBNEWROADFILENAME,
        MI_EBNEWROADNAME,
        MI_EBNEWROADDATAFILENAME,

        NUMBER_OF_MENUITEMS
    };


    MenuPageEditor();
    virtual ~MenuPageEditor();
    void refreshSelected();

    static MenuPageEditor* menuPageEditor;

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshTiles();
    void refreshObjectWire();
    void refreshObjectWireTiles();
    void refreshObjectPool();
    void refreshRaceManager();
    void refreshRoadManager();
    void refreshRoads();
    void refreshRoadEditBoxes(const wchar_t* newRoadName = L"");

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableSelected;
    irr::gui::IGUITable*    tableTiles;
    irr::gui::IGUITable*    tableObjectWire;
    irr::gui::IGUITable*    tableObjectWireTiles;
    irr::gui::IGUITable*    tableObjectPool;
    irr::gui::IGUITable*    tableRaceManager;
    irr::gui::IGUITable*    tableRoadManagerG;
    irr::gui::IGUITable*    tableRoadManagerS;
    irr::gui::IGUITable*    tableRoadManagerV;
    irr::gui::IGUITable*    tableRoadTypes;
    irr::gui::IGUITable*    tableRoads;
    irr::gui::IGUIEditBox*  editBoxNewRoadFilename;
    irr::gui::IGUIEditBox*  editBoxNewRoadName;
    irr::gui::IGUIEditBox*  editBoxNewRoadDataFilename;
};

#endif // MENUPAGEEDITOR_H
