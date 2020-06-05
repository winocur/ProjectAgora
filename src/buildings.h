#ifndef H_BUILDINGS
#define H_BUILDINGS

enum BuildingState {
    IDLE,
    SELECTED,
    DESTROYED
};

enum BuildingType {
    
    BT_NONE,

    // Housing
    BUILDING_BLOCKS,
    SUBURBAN_HOUSES,
    COMMUNAL_HOUSING,
    TECHNO_KAUS,

    // Energy
    COAL_PLANT,
    NATURAL_GAS,
    NUCLEAR_PLANT,
    SOLAR_AND_WIND,

    // Production
    INDUSTRIAL_REFINERY,
    LOCAL_FACTORIES,
    ARTISAN_GUILDS,
    MONDRAGON_COLLECTIVE,

    // Exchange
    BUSINESS_OFFICES,
    SUPERMARKET,
    REGIONAL_BAZAAR,
    POTLACH_GATHERING,
};

struct BuildingData {
    char* name;
    
    int toxicity;
    int housing;
    Resources io;

    int clumpArea;
    int size;

    Color color;
    int height;

    BuildingType evolution;
    BuildingType type;
};

struct Building {
    int instanceId;

    int gridX;
    int gridY;

    BuildingState state;
    BuildingData data;
};

void RenderBuilding (const Building* building, const TileGrid* grid);

Building* QueryBuilding (const Tile* tile, Building* buildings, const u32 buildingCount);

Building* QueryBuildingById (Building* buildings, int buildingCount, int id);

void SortBuildings (const Building* buildings, Building* sortedBuildings, u32 buildingCounter);

Vector2 GetPosition (const Building* building, const TileGrid* grid);

Building CreateBuilding (BuildingType type, int gridX, int gridY);

Building* FilterLiveBuildings (int* outCount, Building* originBuildings, const int originCount, GameMemory* memory); 

BuildingData GetBuildingData (BuildingType type);

#endif