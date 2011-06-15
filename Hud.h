#ifndef HUD_H
#define HUD_H

#include <irrlicht.h>

class ScreenQuad;

class Hud
{
public:
    static void initialize();
    static void finalize();
    
    static Hud* getInstance() {return hud;}

private:
    static Hud* hud;

public:
    Hud();
    ~Hud();
    
    void setVisible(bool newVisible);
    bool getVisible(); // inline
    
    void preRender(float p_angle);
    void render();

private:
    bool            visible;
    ScreenQuad*     miniMapQuad;
    ScreenQuad*     compassQuad;
    ScreenQuad*     tripMasterQuad;
    ScreenQuad*     roadBookBGQuad;

    irr::gui::IGUIStaticText*   compassText;
    irr::gui::IGUIStaticText*   tmPartText;
    irr::gui::IGUIStaticText*   tmTotalText;
};

inline bool Hud::getVisible()
{
    return visible;
}

#endif // HUD_H
