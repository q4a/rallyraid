
#include "Hud.h"
#include "ScreenQuad.h"
#include "TheGame.h"
#include "TheEarth.h"
#include "ShadersSM20.h"

Hud* Hud::hud = 0;

void Hud::initialize()
{
    if (hud == 0)
    {
        hud = new Hud();
    }
}

void Hud::finalize()
{
    if (hud)
    {
        delete hud;
        hud = 0;
    }
}
    

Hud::Hud()
    : visible(true),
      miniMapQuad(0),
      compassQuad(0)
{
    miniMapQuad = new ScreenQuad(TheGame::getInstance()->getDriver(),
        irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width - MINIMAP_SIZE - 10,
            TheGame::getInstance()->getDriver()->getScreenSize().Height - MINIMAP_SIZE - 10),
        irr::core::dimension2du(MINIMAP_SIZE, MINIMAP_SIZE), false);
    miniMapQuad->getMaterial().MaterialType = Shaders::getInstance()->materialMap["quad2d"];

    compassQuad = new ScreenQuad(TheGame::getInstance()->getDriver(),
        irr::core::position2di(10, TheGame::getInstance()->getDriver()->getScreenSize().Height - MINIMAP_SIZE - 10),
        irr::core::dimension2du(MINIMAP_SIZE, MINIMAP_SIZE), false);
    compassQuad->getMaterial().MaterialType = Shaders::getInstance()->materialMap["quad2d_t"];
    compassQuad->getMaterial().setTexture(0, TheGame::getInstance()->getDriver()->getTexture("data/hud/compass.png"));
}

Hud::~Hud()
{
    if (miniMapQuad)
    {
        delete miniMapQuad;
        miniMapQuad = 0;
    }
    
    if (compassQuad)
    {
        delete compassQuad;
        compassQuad = 0;
    }
}
    
void Hud::setVisible(bool newVisible)
{
    if (visible == newVisible) return;
    
    visible = newVisible;
    
    miniMapQuad->setVisible(visible);
    compassQuad->setVisible(visible);
}

void Hud::preRender(float angle)
{
    if (!visible) return;

    miniMapQuad->getMaterial().setTexture(0, TheEarth::getInstance()->getMiniMapTexture());
    compassQuad->rotate(-angle-90.f);
}    
void Hud::render()
{
    if (!visible) return;
    
    miniMapQuad->render();
    compassQuad->render();
}
