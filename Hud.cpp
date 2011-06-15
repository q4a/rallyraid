
#include "Hud.h"
#include "ScreenQuad.h"
#include "TheGame.h"
#include "TheEarth.h"
#include "ShadersSM20.h"
#include "FontManager.h"
#include "Player.h"
#include "Vehicle.h"


// normalize angle between 0 and 360
static float normalizeAngle(float &angle)
{
    while (angle > 360.f) angle -= 360.f;
    while (angle < 0.f) angle += 360.f;
    return angle;
}

static float normalizeAngle180(float &angle)
{
    while (angle > 180.f) angle -= 360.f;
    while (angle < -180.f) angle += 360.f;
    return angle;
}

#define HUD_PADDING             5

#define COMPASS_SIZE            192 // 128
#define COMPASS_HSIZE           (COMPASS_SIZE / 2)
#define COMPASS_TEXT_SIZE_X     42// normalbold: 28, largebold: 34, extralargebold: 42
#define COMPASS_TEXT_HSIZE_X    (COMPASS_TEXT_SIZE_X / 2)
#define COMPASS_TEXT_SIZE_Y     26// normalbold: 18, largebold: 22, extralargebold: 26
#define COMPASS_TEXT_HSIZE_Y    (COMPASS_TEXT_SIZE_Y / 2)

#define COMPASS_WP_ARROW_DIFF   30
#define COMPASS_WP_ARROW_HDIFF  (COMPASS_WP_ARROW_DIFF / 2)
#define WP_ARROW_SIZE           (COMPASS_WP_ARROW_DIFF + COMPASS_SIZE)

#define TM_TEXT_X               (107)
#define TM_PART_TEXT_Y          (78)
#define TM_TOTAL_TEXT_Y         (34)
#define TM_TEXT_SIZE_X          (70)
#define TM_TEXT_SIZE_Y          (22)

#define ROADBOOKBG_SIZE_X       (512)
#define ROADBOOKBG_HSIZE_X      (ROADBOOKBG_SIZE_X / 2)
#define ROADBOOKBG_SIZE_Y       (128)

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
      compassQuad(0),
      tripMasterQuad(0),
      roadBookBGQuad(0),
      compassText(0),
      tmPartText(0),
      tmTotalText(0)
{
    miniMapQuad = new ScreenQuad(TheGame::getInstance()->getDriver(),
        irr::core::position2di(HUD_PADDING, TheGame::getInstance()->getDriver()->getScreenSize().Height - MINIMAP_SIZE - HUD_PADDING),
        irr::core::dimension2du(MINIMAP_SIZE, MINIMAP_SIZE), false);
    miniMapQuad->getMaterial().MaterialType = Shaders::getInstance()->materialMap["quad2d"];

    tripMasterQuad = new ScreenQuad(TheGame::getInstance()->getDriver(),
        irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width - COMPASS_SIZE - COMPASS_WP_ARROW_HDIFF - HUD_PADDING,
            TheGame::getInstance()->getDriver()->getScreenSize().Height - COMPASS_HSIZE - HUD_PADDING),
        irr::core::dimension2du(COMPASS_SIZE, COMPASS_HSIZE), false);
    tripMasterQuad->getMaterial().MaterialType = Shaders::getInstance()->materialMap["quad2d_t"];
    //compassQuad->getMaterial().MaterialTypeParam = 0.2f;
    //compassQuad->getMaterial().MaterialTypeParam2 = 0.8f;
    tripMasterQuad->getMaterial().setFlag(irr::video::EMF_ANTI_ALIASING, false);
    tripMasterQuad->getMaterial().setFlag(irr::video::EMF_BILINEAR_FILTER, false);
    tripMasterQuad->getMaterial().setFlag(irr::video::EMF_TRILINEAR_FILTER, false);
    tripMasterQuad->getMaterial().UseMipMaps = false;
    tripMasterQuad->getMaterial().setTexture(0, TheGame::getInstance()->getDriver()->getTexture("data/hud/tripmaster.png"));

    compassQuad = new ScreenQuad(TheGame::getInstance()->getDriver(),
        irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width - COMPASS_SIZE - COMPASS_WP_ARROW_HDIFF - HUD_PADDING,
            TheGame::getInstance()->getDriver()->getScreenSize().Height - COMPASS_SIZE - COMPASS_HSIZE - COMPASS_WP_ARROW_HDIFF - HUD_PADDING*2),
        irr::core::dimension2du(COMPASS_SIZE, COMPASS_SIZE), false);
    compassQuad->getMaterial().MaterialType = Shaders::getInstance()->materialMap["quad2d_t"];
    //compassQuad->getMaterial().MaterialTypeParam = 0.2f;
    //compassQuad->getMaterial().MaterialTypeParam2 = 0.8f;
    compassQuad->getMaterial().setFlag(irr::video::EMF_ANTI_ALIASING, false);
    compassQuad->getMaterial().setFlag(irr::video::EMF_BILINEAR_FILTER, false);
    compassQuad->getMaterial().setFlag(irr::video::EMF_TRILINEAR_FILTER, false);
    compassQuad->getMaterial().UseMipMaps = false;
    compassQuad->getMaterial().setTexture(0, TheGame::getInstance()->getDriver()->getTexture("data/hud/compass.png"));

    roadBookBGQuad = new ScreenQuad(TheGame::getInstance()->getDriver(),
        irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width/2 - ROADBOOKBG_HSIZE_X,
        TheGame::getInstance()->getDriver()->getScreenSize().Height - ROADBOOKBG_SIZE_Y - HUD_PADDING),
        irr::core::dimension2du(ROADBOOKBG_SIZE_X, ROADBOOKBG_SIZE_Y), false);
    roadBookBGQuad->getMaterial().MaterialType = Shaders::getInstance()->materialMap["quad2d"];
    roadBookBGQuad->getMaterial().setFlag(irr::video::EMF_ANTI_ALIASING, false);
    roadBookBGQuad->getMaterial().setFlag(irr::video::EMF_BILINEAR_FILTER, false);
    roadBookBGQuad->getMaterial().setFlag(irr::video::EMF_TRILINEAR_FILTER, false);
    roadBookBGQuad->getMaterial().UseMipMaps = false;
    roadBookBGQuad->getMaterial().setTexture(0, TheGame::getInstance()->getDriver()->getTexture("data/hud/roadbookbg.png"));

    compassText = TheGame::getInstance()->getEnv()->addStaticText(L"0",
        irr::core::recti(irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width - COMPASS_HSIZE - COMPASS_WP_ARROW_HDIFF - HUD_PADDING - COMPASS_TEXT_HSIZE_X,
        TheGame::getInstance()->getDriver()->getScreenSize().Height - COMPASS_HSIZE - COMPASS_HSIZE - COMPASS_WP_ARROW_HDIFF - HUD_PADDING*2 - COMPASS_TEXT_HSIZE_Y),
        irr::core::dimension2di(COMPASS_TEXT_SIZE_X, COMPASS_TEXT_SIZE_Y)),
        false, false, 0, -1, false);
    compassText->setOverrideFont(FontManager::getInstance()->getFont(FontManager::FONT_EXTRALARGEBOLD));

    tmPartText = TheGame::getInstance()->getEnv()->addStaticText(L"000.00",
        irr::core::recti(irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width - COMPASS_WP_ARROW_HDIFF - HUD_PADDING - TM_TEXT_X,
        TheGame::getInstance()->getDriver()->getScreenSize().Height - HUD_PADDING - TM_PART_TEXT_Y),
        irr::core::dimension2di(TM_TEXT_SIZE_X, TM_TEXT_SIZE_Y)),
        false, false, 0, -1, false);
    tmPartText->setOverrideFont(FontManager::getInstance()->getFont(FontManager::FONT_LARGE));

    tmTotalText = TheGame::getInstance()->getEnv()->addStaticText(L"000.00",
        irr::core::recti(irr::core::position2di(TheGame::getInstance()->getDriver()->getScreenSize().Width - COMPASS_WP_ARROW_HDIFF - HUD_PADDING - TM_TEXT_X,
        TheGame::getInstance()->getDriver()->getScreenSize().Height - HUD_PADDING - TM_TOTAL_TEXT_Y),
        irr::core::dimension2di(TM_TEXT_SIZE_X, TM_TEXT_SIZE_Y)),
        false, false, 0, -1, false);
    tmTotalText->setOverrideFont(FontManager::getInstance()->getFont(FontManager::FONT_LARGE));
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

    if (tripMasterQuad)
    {
        delete tripMasterQuad;
        tripMasterQuad = 0;
    }

    if (roadBookBGQuad)
    {
        delete roadBookBGQuad;
        roadBookBGQuad = 0;
    }
}
    
void Hud::setVisible(bool newVisible)
{
    if (visible == newVisible) return;
    
    visible = newVisible;
    
    miniMapQuad->setVisible(visible);
    compassQuad->setVisible(visible);
    tripMasterQuad->setVisible(visible);
    roadBookBGQuad->setVisible(visible);

    compassText->setVisible(visible);
    tmPartText->setVisible(visible);
    tmTotalText->setVisible(visible);
}

void Hud::preRender(float p_angle)
{
    if (!visible) return;

    float angle = -p_angle-90.f;
    irr::core::stringw str;

    miniMapQuad->getMaterial().setTexture(0, TheEarth::getInstance()->getMiniMapTexture());
    compassQuad->rotate(angle);

    str = L"";
    unsigned int angleInt = 360 - (unsigned int)normalizeAngle(angle);
    if (angleInt < 10)
    {
        str += L"00";
    }
    else
    if (angleInt < 100)
    {
        str += L"0";
    }
    str += angleInt;
    compassText->setText(str.c_str());

    str = L"";
    unsigned int dist = (unsigned int)(Player::getInstance()->getDistance()/1000.f) % 1000;
    unsigned int distp = (unsigned int)(Player::getInstance()->getDistance()/10.f) % 100;
    if (dist < 10)
    {
        str += L"00";
    }
    else
    if (dist < 100)
    {
        str += L"0";
    }
    str += dist;
    str += L".";
    if (distp < 10)
    {
        str += L"0";
    }
    str += distp;
    tmPartText->setText(str.c_str());

    str = L"";
    dist = (unsigned int)(Player::getInstance()->getVehicle()->getDistance()/1000.f) % 1000;
    distp = (unsigned int)(Player::getInstance()->getVehicle()->getDistance()/10.f) % 100;
    if (dist < 10)
    {
        str += L"00";
    }
    else
    if (dist < 100)
    {
        str += L"0";
    }
    str += dist;
    str += L".";
    if (distp < 10)
    {
        str += L"0";
    }
    str += distp;
    tmTotalText->setText(str.c_str());
}
void Hud::render()
{
    if (!visible) return;
    
    miniMapQuad->render();
    compassQuad->render();
    tripMasterQuad->render();
    roadBookBGQuad->render();
}
