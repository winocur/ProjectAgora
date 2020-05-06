#include "simulation.h"

GameSession StartGame() {

    GameSession session;
    session.population = 300000;
    session.resources = { 200, 200, 200};
    session.timer = 0;
    session.buildingCounter = 0;
    session.natureCounter = 0;
    
    // initial buildings
    session.buildings[session.buildingCounter] = CreateBuilding(1, 1, Color { 0, 255, 125, 200 }, 8, 8, 50);
    session.buildingCounter++;

    session.buildings[session.buildingCounter] = CreateBuilding(2, 2, Color { 255, 0, 125, 200 }, 2, 8, 30);
    session.buildingCounter++;

    session.buildings[session.buildingCounter] = CreateBuilding(3, 1, Color { 125, 0, 125, 200 }, 2, 3, 80);
    session.buildingCounter++;

    session.phase = GP_IDLE;

    return session;
}

void ProcessTick (GameSession* session) {
    printf ("Tick!\n");

    session->timer = 0;
    session->population -= (int)(GetToxicity(session) * 1000);

    session->resources = session->resources + GetResourceIO(session);
}

float GetToxicity (const GameSession* session) {

    float toxicity = 0;

    for (int i = 0; i < session->buildingCounter; i++) {

        Building building = session->buildings[i];
        toxicity += building.data.toxicity;
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