
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
        MI_BUTTONACTIVATE,
        MI_BUTTONRESET,
        MI_BUTTONRELOAD,
        MI_TABCONTROL,

        MI_TABSELECTED,
        MI_TABTILES,
        MI_TABOBJECTWIRE,
        MI_TABOBJECTWIRETILES,
        MI_TABOBJECTPOOL,
        MI_TABRACEMANAGER,
        MI_TABROADMANAGER,
        MI_TABROADS,
        MI_TABITINER,

        MI_TABLESELECTED,
        MI_TABLEACTION,
        MI_TABLETILES,
        MI_TABLEOBJECTWIRE,
        MI_TABLEOBJECTWIRETILES,
        MI_TABLEOBJECTPOOL,
        MI_TABLERACEMANAGER,
        MI_TABLEROADMANAGERS,
        MI_TABLEROADMANAGERV,
        MI_TABLEROADTYPES,
        MI_TABLEROADS,
        MI_TABLEITINER,
        MI_TABLEITINER2,

        MI_EBNEWROADFILENAME,
        MI_EBNEWROADNAME,
        MI_EBNEWROADDATAFILENAME,
        MI_EBITINERGD,
        MI_EBITINERLD,
        MI_EBITINERDESCRIPTION,
        MI_EBRESETX,
        MI_EBRESETY,
        MI_EBRESETZ,

        MI_CBRENDER,
        
        MI_STITINERGD,

        NUMBER_OF_MENUITEMS
    };

    enum Action
    {
        A_None = 0,
        A_AddObjectGlobal,
        A_AddObjectRace,
        A_AddObjectDay,
        A_AddObjectStage,
        A_AddRoadPoint,
        A_AddRoadPointBegin,
        A_AddItinerPoint,
        A_AddAIPoint,
        A_AddWayPoint,
        A_AddHeightModifier,
        A_RemoveObjectGlobal,
        A_RemoveObjectRace,
        A_RemoveObjectDay,
        A_RemoveObjectStage,
        A_RemoveRoadPoint,
        A_RemoveRoadPointBegin,
        A_RemoveItinerPoint,
        A_RemoveAIPoint,
        A_RemoveWayPoint,
    };

    MenuPageEditor();
    virtual ~MenuPageEditor();
    void refreshSelected();
    void refreshItinerGD();

    // mouse button hit
    static void action();
    // render editor stuff
    static void render();

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
    void refreshItiner();

    void actionP();
    void renderP();

private:
    irr::gui::IGUIWindow*   window;
    irr::gui::IGUITable*    tableSelected;
    irr::gui::IGUITable*    tableAction;
    irr::gui::IGUITable*    tableTiles;
    irr::gui::IGUITable*    tableObjectWire;
    irr::gui::IGUITable*    tableObjectWireTiles;
    irr::gui::IGUITable*    tableObjectPool;
    irr::gui::IGUITable*    tableRaceManager;
    irr::gui::IGUITable*    tableRoadManagerS;
    irr::gui::IGUITable*    tableRoadManagerV;
    irr::gui::IGUITable*    tableRoadTypes;
    irr::gui::IGUITable*    tableRoads;
    irr::gui::IGUITable*    tableItiner;
    irr::gui::IGUITable*    tableItiner2;
    irr::gui::IGUIEditBox*  editBoxNewRoadFilename;
    irr::gui::IGUIEditBox*  editBoxNewRoadName;
    irr::gui::IGUIEditBox*  editBoxNewRoadDataFilename;
    irr::gui::IGUIEditBox*  editBoxItinerGD;
    irr::gui::IGUIEditBox*  editBoxItinerLD;
    irr::gui::IGUIEditBox*  editBoxItinerDescription;
    irr::gui::IGUIEditBox*  editBoxResetX;
    irr::gui::IGUIEditBox*  editBoxResetY;
    irr::gui::IGUIEditBox*  editBoxResetZ;
    irr::gui::IGUICheckBox* checkBoxRender;
    irr::gui::IGUIStaticText* staticTextItinerGD;

    Action                  currentAction;
    irr::video::SMaterial   material;
    unsigned int            lastTick;
    bool                    doRender;
};

#endif // MENUPAGEEDITOR_H
