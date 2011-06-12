
#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <map>
#include <list>
#include <string>
#include <irrlicht.h>

#define SAVE_DIR                (std::string("savegames"))
#define SAVE_STATE(saveName)    (SAVE_DIR + "/" + saveName + "/" + std::string("racestate"))
#define SAVE_ENGINE(saveName)   (SAVE_DIR + "/" + saveName + "/" + std::string("stagestate"))
#define SAVE_PLAYER(saveName)   (SAVE_DIR + "/" + saveName + "/" + std::string("player"))

class Stage;
class Race;
class VehicleType;
class Competitor;
class RaceEngine;

class CompetitorResult
{
public:
    CompetitorResult(Competitor* competitor)
        : competitor(competitor),
          stageTime(0),
          stagePenalityTime(0),
          globalTime(0),
          globalPenalityTime(0)
    {
    }

    CompetitorResult(Competitor* competitor, unsigned int stageTime, unsigned int stagePenalityTime,
        unsigned int globalTime, unsigned int globalPenalityTime)
        : competitor(competitor),
          stageTime(stageTime),
          stagePenalityTime(stagePenalityTime),
          globalTime(globalTime),
          globalPenalityTime(globalPenalityTime)
    {
    }

    Competitor*     competitor;
    unsigned int    stageTime;
    unsigned int    stagePenalityTime;
    unsigned int    globalTime;
    unsigned int    globalPenalityTime;
};
typedef std::list<CompetitorResult*> competitorResultList_t;

// -----------------------------------------------------------------

class StageState
{
public:
    Stage*                  stage;
    competitorResultList_t  competitorResultListStage;
    competitorResultList_t  competitorResultListOverall;
};
typedef std::list<StageState*> stageStateList_t;

// -----------------------------------------------------------------

class GamePlay
{
public:
    static void initialize();
    static void finalize();
    
    static GamePlay* getInstance() {return gamePlay;}

private:
    static GamePlay* gamePlay;

private:
    GamePlay();
    ~GamePlay();

public:
    void startNewGame(Race* race, VehicleType* vehicleType);
    bool loadGame(const std::string& saveName);
    bool saveGame(const std::string& saveName);

    // called by the TheGame::loop()
    void update(unsigned int tick, const irr::core::vector3df& apos);

    unsigned int competitorFinished(CompetitorResult* competitorResult);

private:
    void startStage(Stage* stage, VehicleType* vehicleType);

private:
    static bool readStageStateList(const std::string& filename, stageStateList_t& stageStateList);
    static bool writeStageStateList(const std::string& filename, const stageStateList_t& stageStateList);
    static bool readCompetitorResultList(FILE* f, Race* race, competitorResultList_t& competitorResultList);
    static bool writeCompetitorResultList(FILE* f, const competitorResultList_t& competitorResultList);

    static void clearStageStateList(stageStateList_t& stageStateList);
    static void clearCompetitorResultList(competitorResultList_t& competitorResultList);

private:
    Race*               currentRace;
    Stage*              currentStage;
    stageStateList_t    raceState;
    RaceEngine*         raceEngine;


    friend class MenuPageEditor;
    friend class MenuPageEditorRoad;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
    friend class MenuPageMain;
};


#endif // GAMEPLAY_H
