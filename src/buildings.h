#ifndef H_BUILDINGS
#define H_BUILDINGS

enum BuildingState {
    IDLE,
    SELECTED,
    DESTROYED
};

struct BuildingData {
    char* name;

    int toxicity;
    int housing;

    Resources io;

    int clumpArea;

    int buildingClassId;
    int evolvesIntoClassId;
};

struct Building {

    int instanceId;
    int classId;

    Color color;

    int gridX;
    int gridY;

    // radius
    int size;

    int roofHeight;

    BuildingState state;

    BuildingData data;
};

void RenderBuilding (const Building* building, const TileGrid* grid);

Building* QueryBuilding (const Tile* tile, Building* buildings, const u32 buildingCount);

Building* QueryBuildingById (Building* buildings, int buildingCount, int id);

void SortBuildings (const Building* buildings, Building* sortedBuildings, u32 buildingCounter);

Vector2 GetPosition (const Building* building, const TileGrid* grid);

Building CreateBuilding (int classId, int size, Color color, int gridX, int gridY, int roofHeight);

Building* FilterLiveBuildings (int* outCount, Building* originBuildings, const int originCount, GameMemory* memory); 

BuildingData GetBuildingData (int classId);

#endif