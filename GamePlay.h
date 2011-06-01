
#ifndef GAMEPLAY_H
#define GAMEPLAY_H


class Stage;

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
    void startGame(Stage* stage);

private:


    friend class MenuPageEditor;
    friend class MenuPageEditorRoad;
    friend class MenuPageEditorRace;
    friend class MenuPageEditorDay;
    friend class MenuPageEditorStage;
};


#endif // GAMEPLAY_H
