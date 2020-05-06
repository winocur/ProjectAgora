#ifndef H_SIMULATION
#define H_SIMULATION

struct Nature {

    u32 level;

    static const float toxicityFactor = 3.0;
};

struct GameSession {
    static const float tickTime = 10000.0; 

    u32 population;

    Resources resources;

    f64 timer;
    
    Building buildings [32];
    u32 buildingCounter; 

    Nature natures [32];
    u32 natureCounter; 

    GamePhase phase;
};

GameSession StartGame();

void ProcessTick (GameSession* session);

float GetToxicity (const GameSession* session);

Resources GetResourceIO (const GameSession* session);

u32 GetHousing (const GameSession* session);

#endif