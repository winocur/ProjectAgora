#include "level.h"

void LoadLevel (GameSession* session) {

    const int gridSize = 16;

    SDL_RWops *file = SDL_RWFromFile("../assets/levels/level_0.txt", "rb");

    if(file == NULL) {
        printf("no encontre el archivo");
        return;
    }

    char content[512];
    size_t size = file->read(file, content, 1, 512);
    printf("level file has %i chars\n", size);

    int charactersVisited = 0;

    for (size_t i = 0; i < size; i++) {

        char character = content[i];

        if(charactersVisited >= gridSize * gridSize) break;

        if(character == '-' || character == 'o') {
            charactersVisited++;
            continue;
        }

        switch (character) {
            case 'B' : {
                session->buildings[session->buildingCounter] = CreateBuilding(BUILDING_BLOCKS, charactersVisited % gridSize, charactersVisited / gridSize);
                session->buildingCounter++;
                charactersVisited++;
            } break;

            case 'C' : {
                session->buildings[session->buildingCounter] = CreateBuilding(COAL_PLANT, charactersVisited % gridSize, charactersVisited / gridSize);
                session->buildingCounter++;
                charactersVisited++;
            } break;

            case 'I' : {
                session->buildings[session->buildingCounter] = CreateBuilding(INDUSTRIAL_REFINERY, charactersVisited % gridSize, charactersVisited / gridSize);
                session->buildingCounter++;
                charactersVisited++;
            } break;
 
            case 'O' : {
                session->buildings[session->buildingCounter] = CreateBuilding(BUSINESS_OFFICES, charactersVisited % gridSize, charactersVisited / gridSize);
                session->buildingCounter++;
                charactersVisited++;
            } break;

        }
    }
        
    file->close(file);
}