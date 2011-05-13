
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

        MI_TABSELECTED,
        MI_TABTILES,
        MI_TABOBJECTWIRE,
        MI_TABOBJECTWIRETILES,
        MI_TABOBJECTPOOL,
        MI_TABRACEMANAGER,
        MI_TABROADMANAGER,

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
    void refreshSelected();
    void refreshTiles();
    void refreshObjectWire();
    void refreshObjectWireTiles();
    void refreshObjectPool();
    void refreshRaceManager();
    void refreshRoadManager();

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
};

#endif // MENUPAGEEDITOR_H
