
#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <map>
#include <list>
#include <string>

class Stage;
class Race;
class VehicleType;
class Competitor;

struct CompetitorResult
{
    CompetitorResult(Competitor* competitor)
        : competitor(competitor),
          stageTime(0),
          globalTime(0)
    {
    }

    CompetitorResult(Competitor* competitor, unsigned int stageTime, unsigned int globalTime)
        : competitor(competitor),
          stageTime(stageTime),
          globalTime(globalTime)
    {
    }

    Competitor*     competitor;
    unsigned int    stageTime;
    unsigned int    globalTime;
};
typedef std::list<CompetitorResult*> competitorResultList_t;

struct StageState
{
    Stage*                  stage;
    competitorResultList_t  competitorResultListStage;
    competitorResultList_t  competitorResultListOverall;
};
typedef std::list<StageState*> stageStateList_t;

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
    void startGame(Stage* stage, VehicleType* vehicleType);

private:
    static bool readStageStateList(const std::string& filename, stageStateList_t& stageStateList);
    static bool writeStageStateList(const std::string& filename, const stageStateList_t& stageStateList);
    static bool readCompetitorResultList(FILE* f, Race* race, competitorResultList_t& competitorResultList);
    static bool writeCompetitorResultList(FILE* f, const competitorResultList_t& competitorResultList);

private:


    friend class MenuPageEditor;
    friend class MenuPageEditorRoad;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
    friend class MenuPageMain;
};


#endif // GAMEPLAY_H
