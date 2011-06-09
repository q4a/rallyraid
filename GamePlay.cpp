
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
#include "VehicleType.h"
#include "Competitor.h"
#include <assert.h>


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

void GamePlay::startGame(Stage* stage, VehicleType* vehicleType)
{
    irr::core::vector3df initialPos(4190225.f, 215.f, -6401350.f);
    irr::core::vector3df initialDir(1.f, 0.f, 0.f);
    float deg = 0.f;
    
    dprintf(MY_DEBUG_INFO, "GamePlay::startGame(): stage: %p, vehicleType: %p\n", stage, vehicleType);
    assert(vehicleType);

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
    Player::getInstance()->initializeVehicle(vehicleType->getName(), initialPos+initialDir, irr::core::vector3df(0.f, deg, 0.f));
    
    if (stage)
    {
        RaceManager::getInstance()->activateStage(stage);
    }

    TheGame::getInstance()->resetTick();
}

/* static */ bool GamePlay::readStageStateList(const std::string& filename, stageStateList_t& stageStateList)
{
    FILE* f;
    int ret = 0;
    char raceName[256];
    char dayName[256];
    char stageName[256];
    Race* race = 0;
    Day* day = 0;
    Stage* stage = 0;
    unsigned long numOfStages;


    if (!stageStateList.empty())
    {
        dprintf(MY_DEBUG_WARNING, "stage state already exists: %s\n", filename.c_str());
        return true;
    }

    errno_t error = fopen_s(&f, filename.c_str(), "r");
    if (error)
    {
        printf("stage state file unable to open: %s\n", filename.c_str());
        return false;
    }
    
    ret = fscanf_s(f, "%s\n", raceName);
    if (ret < 1)
    {
        printf("stage state unable to read race name: %s\n", filename.c_str());
        fclose(f);
        return false;
    }
    race = RaceManager::getInstance()->getRace(raceName);
    if (race == 0)
    {
        printf("unable to find race: %s\n", raceName);
        fclose(f);
        return false;
    }

    ret = fscanf_s(f, "%lu\n", &numOfStages);
    if (ret < 1)
    {
        printf("stage state unable to read number of stages: %s\n", filename.c_str());
        fclose(f);
        return false;
    }

    for (unsigned int i = 0; i < numOfStages; i++)
    {
        ret = fscanf_s(f, "%s\n%s\n", stageName, dayName);
        if (ret < 2)
        {
            printf("stage state unable to read stage and day name: %s\n", filename.c_str());
            fclose(f);
            return false;
        }
        StageState* stageState = new StageState;
        day = race->getDay(dayName);
        if (day == 0)
        {
            printf("unable to find day: %s (%s)\n", dayName, raceName);
            fclose(f);
            return false;
        }
        stage = day->getStage(stageName);
        if (stage == 0)
        {
            printf("unable to find stage: %s (%s, %s)\n", stageName, dayName, raceName);
            fclose(f);
            return false;
        }
        stageState->stage = stage;

        bool retb = readCompetitorResultList(f, race, stageState->competitorResultListStage);
        if (!retb)
        {
            printf("unable to read competitor list stage\n");
            fclose(f);
            return false;
        }
        retb = readCompetitorResultList(f, race, stageState->competitorResultListOverall);
        if (!retb)
        {
            printf("unable to read competitor list overall\n");
            fclose(f);
            return false;
        }

        stageStateList.push_back(stageState);
    }
    fclose(f);
    return true;
}

/* static */ bool GamePlay::writeStageStateList(const std::string& filename, const stageStateList_t& stageStateList)
{
    FILE* f;
    int ret = 0;
    Race* race = 0;
    Day* day = 0;
    Stage* stage = 0;

    if (stageStateList.empty())
    {
        return true;
    }

    stage = stageStateList.front()->stage;
    day = stage->getParent();
    race = day->getParent();

    errno_t error = fopen_s(&f, filename.c_str(), "w");
    if (error)
    {
        printf("unable to open stage state file for write %s\n", filename.c_str());
        return false;
    }

    ret = fprintf(f, "%s\n", race->getName().c_str());

    ret = fprintf(f, "%lu\n", stageStateList.size());
    for (stageStateList_t::const_iterator it = stageStateList.begin(); 
         it != stageStateList.end();
         it++)
    {
        stage = (*it)->stage;
        day = stage->getParent();
        assert(race == day->getParent());
        ret = fprintf(f, "%s\n", stage->getName().c_str());
        ret = fprintf(f, "%s\n", day->getName().c_str());
        writeCompetitorResultList(f, (*it)->competitorResultListStage);
        writeCompetitorResultList(f, (*it)->competitorResultListOverall);
    }

    fclose(f);
    return true;
}

/* static */ bool GamePlay::readCompetitorResultList(FILE* f, Race* race, competitorResultList_t& competitorResultList)
{
    int ret = 0;
    unsigned long numOfComps = 0;
    unsigned int num = 0;
    unsigned int stageTime = 0;
    unsigned int globalTime = 0;

    ret = fscanf_s(f, "%lu\n", &numOfComps);
    if (ret < 1)
    {
        printf("stage state unable to read number of competitor list\n");
        fclose(f);
        return false;
    }

    for (unsigned int i = 0; i < numOfComps; i++)
    {
        ret = fscanf_s(f, "%u %u %u\n", &num, &stageTime, &globalTime);
        if (ret < 3)
        {
            printf("stage state unable to read competitor list\n");
            fclose(f);
            return false;
        }
        Competitor* competitor = 0;
        Race::competitorMap_t::const_iterator cit = race->getCompetitorMap().find(num);
        if (cit != race->getCompetitorMap().end())
        {
            competitor = cit->second;
        }
        else
        {
            printf("stage state unable to find competitor: %u (%s)\n", num, race->getName());
            fclose(f);
            return false;
        }
        CompetitorResult* competitorResult = new CompetitorResult(competitor, stageTime, globalTime);

        competitorResultList.push_back(competitorResult);
    }
    return true;
}

/* static */ bool GamePlay::writeCompetitorResultList(FILE* f, const competitorResultList_t& competitorResultList)
{
    int ret = 0;
    ret = fprintf(f, "%lu\n", competitorResultList.size());

    for (competitorResultList_t::const_iterator it = competitorResultList.begin(); 
         it != competitorResultList.end();
         it++)
    {
        ret = fprintf(f, "%u %u %u\n", (*it)->competitor->getNum(), (*it)->stageTime, (*it)->globalTime);
    }
    return true;
}
