#ifndef H_SIMULATION
#define H_SIMULATION

struct Nature {

    u32 level;

    static constexpr  float toxicityFactor = 3.0;
};

struct GameSession {
    static constexpr  float tickTime = 10000.0; 

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

Resources GetUpgradeCost (const Building* building);

Resources GetDemolisionCost (const Building* building);

Resources GetMoveCost (const Building* building);

bool UpgradeBuilding (Building* building);

bool DemolishBuilding (Building* building);

bool StartMovingBuilding (Building* building);

Building* PlaceBuilding (Building building);

#endif