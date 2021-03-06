
#ifndef MENUPAGEINGAME_H
#define MENUPAGEINGAME_H

#include "MenuPageBase.h"
#include "MenuManager.h"


class StageState;

class MenuPageInGame : public MenuPageBase
{
public:
    enum MenuItemId
    {
        MI_WINDOW = MAX_MENU_ITEMS * MenuManager::MP_INGAME,
        MI_BUTTONBACK,
        MI_BUTTONLOAD,
        MI_BUTTONSAVE,
        MI_BUTTONOPTIONS,
        MI_BUTTONEXIT,
        MI_BUTTONNEXTSTAGE,

        MI_TABLESTAGES,
        MI_TABLECOMPETITORS,
        MI_TABLECOMPETITORSG,
        MI_TABLECOMPETITORSINSTAGE,

        NUMBER_OF_MENUITEMS
    };


    MenuPageInGame();
    virtual ~MenuPageInGame();

    static MenuPageInGame* menuPageInGame;

protected:
    virtual bool OnEvent (const irr::SEvent &event);
    virtual void open();
    virtual void close();

private:
    void refresh();
    void refreshStages();
    void refreshCompetitors(StageState* stageState);

private:
    irr::gui::IGUIImage*    window;
    irr::gui::IGUIStaticText* staticTextRaceName;
    irr::gui::IGUITable*    tableStages;
    irr::gui::IGUITable*    tableCompetitors;
    irr::gui::IGUITable*    tableCompetitorsG;
    irr::gui::IGUITable*    tableCompetitorsInStage;
    irr::gui::IGUIButton*   buttonLoad;
    irr::gui::IGUIButton*   buttonSave;
    irr::gui::IGUIButton*   buttonNextStage;

    bool                    willOpenOtherWindow;
};

#endif // MENUPAGEINGAME_H
