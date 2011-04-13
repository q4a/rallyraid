#ifndef HUD_H
#define HUD_H

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
    
    void preRender(float angle);
    void render();

private:
    bool            visible;
    ScreenQuad*     miniMapQuad;
    ScreenQuad*     compassQuad;
};

inline bool Hud::getVisible()
{
    return visible;
}

#endif // HUD_H
