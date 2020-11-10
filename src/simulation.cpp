#include "simulation.h"

GameSession StartGame() {

    GameSession session;
    session.population = 300000;
    session.resources = { 200, 200, 200};
    session.timer = 0;
    session.buildingCounter = 0;
    session.natureCounter = 0;
    
    // initial buildings
    LoadLevel(&session);

    session.phase = GP_IDLE;

    return session;
}

void ProcessTick (GameSession* session) {
    session->timer = 0;
    session->population -= (int)(GetToxicity(session) * 1000);

    session->resources = session->resources + GetResourceIO(session);
}

float GetToxicity (const GameSession* session) {

    float toxicity = 0;

    for (int i = 0; i < session->buildingCounter; i++) {

        Building building = session->buildings[i];
        toxicity += GetBuildingToxicity(&building);
    }

    return toxicity;

}

Resources GetResourceIO (const GameSession* session) {

    Resources resources = Resources {0, 0, 0};

    for (int i = 0; i < session->buildingCounter; i++) {

        Building building = session->buildings[i];
        resources = resources + building.data.io;
    }

    printf("io: en: %i, pro: %i, ex: %i\n", resources.energy, resources.production, resources.exchange);

    return resources;
}

Resources GetUpgradeCost (const Building* building) {
    return Resources { 200, 100, 100 };
}

Resources GetDemolisionCost (const Building* building) {
    return Resources { 150, 20, 20 };
}

Resources GetMoveCost (const Building* building) {
    return Resources { 80, 50, 50 };
}

bool StartMovingBuilding (Building* building) {

    GameSession* session = &gameState->session;

    Resources cost = GetMoveCost(building);
    if(HasEnoughResources(session->resources, cost)) {
        session->resources = session->resources - cost;
        session->phase = GP_MOVING_BUILDING;
        building->state = MOVING;
    }
}

bool DemolishBuilding (Building* building) {

    GameSession* session = &gameState->session;

    Resources cost = GetDemolisionCost(building);
    if(HasEnoughResources(session->resources, cost)) {
        session->resources = session->resources - cost;
        building->state = DESTROYED;
        return true;
    }

    return false;
}

bool UpgradeBuilding (Building* building) {

    GameSession* session = &gameState->session;

    Resources cost = GetUpgradeCost(building);
    if(HasEnoughResources(session->resources, cost)) {
        session->resources = session->resources - cost;

        if(building->data.evolution == BT_NONE) {
            printf("Building has no evolution");
            return false;
        }

        BuildingType type = building->data.evolution;
        building->data = GetBuildingData(type);
        return true;
    }

    return false;
}

Building* PlaceBuilding (Building building) {

    GameSession* session = &gameState->session;

    for (int i = 0; i < session->buildingCounter; i++) {
        if (session->buildings[i].state == DESTROYED) {
            session->buildings[i] = building;
            return session->buildings + i;
        }
    }

    session->buildings[session->buildingCounter] = building;
    session->buildingCounter++;
    return session->buildings + (session->buildingCounter - 1);
}
