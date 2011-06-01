
#include "GamePlay.h"
#include "stdafx.h"
#include "RaceManager.h"
#include "Race.h"
#include "Day.h"
#include "Stage.h"
#include "TheGame.h"
#include "TheEarth.h"
#include "ObjectWire.h"
#include "Player.h"


GamePlay* GamePlay::gamePlay = 0;

void GamePlay::initialize()
{
    if (gamePlay == 0)
    {
        gamePlay = new GamePlay();
    }
}

void GamePlay::finalize()
{
    if (gamePlay)
    {
        delete gamePlay;
        gamePlay = 0;
    }
}


GamePlay::GamePlay()
{
}

GamePlay::~GamePlay()
{
}

void GamePlay::startGame(Stage* stage)
{
    irr::core::vector3df initialPos(4190225.f, 215.f, -6401350.f);
    irr::core::vector3df initialDir(1.f, 0.f, 0.f);
    float deg = 0.f;
    
    dprintf(MY_DEBUG_INFO, "GamePlay::startGame(): stage: %p\n", stage);

    if (stage)
    {
        unsigned int size = stage->getAIPointList().size();
        
        if (size > 0)
        {
            AIPoint::AIPointList_t::const_iterator aiit = stage->getAIPointList().begin();
            initialPos = (*aiit)->getPos();
            
            if (size > 1)
            {
                aiit++;
                initialDir = (*aiit)->getPos() - initialPos;
                initialDir.normalize();
                
                irr::core::vector2df toDeg(initialDir.X, initialDir.Z);
                deg = (float)toDeg.getAngle();
            }
            
            initialPos.Y += 2.5f;
        }
    }

    ObjectWire::getInstance()->reset();
    TheGame::getInstance()->reset(initialPos, initialDir);
    TheEarth::getInstance()->createFirst(initialPos, initialDir);
    Player::getInstance()->finalizeVehicle();
    Player::getInstance()->initializeVehicle(initialPos+initialDir, irr::core::vector3df(0.f, deg, 0.f));
    
    if (stage)
    {
        RaceManager::getInstance()->activateStage(stage);
    }

    TheGame::getInstance()->resetTick();
}
