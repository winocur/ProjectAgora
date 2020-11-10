#include "buildings.h"


void RenderBuilding (const Building* building, const TileGrid* grid, GameMemory* gameMemory) {

    Tile tile = grid->tiles[building->gridX][building->gridY];

    int roof = building->data.height;
    int size = building->data.size;

    Vector2 bottomLeft = ScreenToProjection( { tile.center.x - grid->tileWidth / 2  - size * grid->tileWidth, tile.center.y - grid->tileHeight / 2 - size * grid->tileHeight }); 
    Vector2 topLeft    = ScreenToProjection( { tile.center.x - grid->tileWidth / 2     - size * grid->tileWidth, tile.center.y + grid->tileHeight / 2 + size * grid->tileHeight }); 
    Vector2 topRight   = ScreenToProjection( { tile.center.x + grid->tileWidth / 2    + size * grid->tileWidth, tile.center.y + grid->tileHeight / 2 + size * grid->tileHeight }); 
    Vector2 bottomRight = ScreenToProjection( { tile.center.x + grid->tileWidth / 2 + size * grid->tileWidth, tile.center.y - grid->tileHeight / 2 - size * grid->tileHeight });

    Color baseColor = building->data.color;
    Color midColor  = baseColor + Color { - 50, -50, -50, 0};
    Color darkColor = baseColor + Color { - 100, -100, -100, 0};

    if(building->state == SELECTED) {

        glBegin (GL_QUADS);
        glColor4ub (255, 255, 125, 255);

        glVertex3f (bottomLeft.x, bottomLeft.y,  0);
        glVertex3f (topLeft.x, topLeft.y, 0);
        glVertex3f (topRight.x, topRight.y,  0);
        glVertex3f (bottomRight.x, bottomRight.y, 0);
        glVertex3f (bottomLeft.x, bottomLeft.y,  0);
        glEnd();

        RenderBuildingClumping(building, gameMemory);
    }

    glBegin (GL_QUADS);

    glColor4ub (darkColor.r, darkColor.g, darkColor.b, darkColor.a);

    //bottom face
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glVertex3f (topLeft.x, topLeft.y, 0);
    glVertex3f (topRight.x, topRight.y,  0);
    glVertex3f (bottomRight.x, bottomRight.y, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glEnd();
    
    //top face
    glBegin (GL_QUADS);

    glColor4ub (midColor.r, midColor.g, midColor.b, 230);

    glVertex3f (bottomLeft.x, bottomLeft.y + roof,  0);
    glVertex3f (topLeft.x, topLeft.y + roof, 0);
    glVertex3f (topRight.x, topRight.y + roof,  0);
    glVertex3f (bottomRight.x, bottomRight.y + roof, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y + roof,  0);
    glEnd();
    
    //left wall
    glBegin (GL_QUADS);

    glColor4ub (baseColor.r, baseColor.g, baseColor.b, baseColor.a);

    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glVertex3f (bottomLeft.x, bottomLeft.y + roof,  0);
    glVertex3f (bottomRight.x, bottomRight.y + roof, 0);
    glVertex3f (bottomRight.x, bottomRight.y, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    
    glEnd();
    
    //right
    glBegin (GL_QUADS);
    glColor4ub (midColor.r, midColor.g, midColor.b, midColor.a);

    glVertex3f (topLeft.x, topLeft.y,  0);
    glVertex3f (topLeft.x, topLeft.y + roof,  0);
    glVertex3f (topRight.x, topRight.y + roof, 0);
    glVertex3f (topRight.x, topRight.y, 0);
    glVertex3f (topLeft.x, topLeft.y,  0);
    
    glEnd();

}

void RenderBuildingClumping (const Building* building, GameMemory* gameMemory) {

    int clumpingTiles = 0;
    Tile* tiles = GetClumpingTiles(&gameState->grid, building->data.clumpArea, building->gridX, building->gridY, building->data.size, &clumpingTiles, gameMemory);

    for (int i = 0; i < clumpingTiles; i++) {
        HighlightTile(tiles[i], { 200, 200, 0, 150 });
    }

}

void RenderBuildingCollision (TileGrid* grid, Building* building) {

    int size = building->data.size;
    for (int i = building->gridX - size; i <= building->gridX + size; i++)
    {
        for (int j = building->gridY - size; j <= building->gridY + size; j++) {

            Tile tile = grid->tiles[i] [j];
            Building* other = QueryBuilding(&tile, gameState->session.buildings, gameState->session.buildingCounter);

            if(other != NULL) {
                HighlightTile(tile, { 250, 0, 0, 250} );
            }
        }
    }
}

bool ValidateBuildingPosition (Building* building) {
    
    int size = building->data.size;
    for (int i = building->gridX - size; i <= building->gridX + size; i++)
    {
        for (int j = building->gridY - size; j <= building->gridY + size; j++) {

            Tile tile = grid->tiles[i] [j];
            Building* other = QueryBuilding(&tile, gameState->session.buildings, gameState->session.buildingCounter);

            if(other != NULL) {
                return false;
            }
        }
    }

    return true;
}

Vector2 GetPosition (const Building* building, const TileGrid* grid) {

    Tile tile = grid->tiles[building->gridX][building->gridY];
    return Vector2 { tile.center.x, tile.center.y };
}

Building* QueryBuilding (const Tile* tile, Building* buildings, const u32 buildingCount) {

    for (int i = 0; i < buildingCount; i++) {

        Building* building = buildings + i;

        if(building->state == DESTROYED || building->state == MOVING) continue;
        
        int size = building->data.size;

        int relativeGridX = abs(building->gridX - tile->indexX);
        int relativeGridY = abs(building->gridY - tile->indexY);

        if(relativeGridX <= size  && relativeGridY <= size) return building;
    }

    return NULL;
}

Building* QueryBuildingById (Building* buildings, int buildingCount, int id) {
    
    if (id == -1) return NULL;

    for (int i = 0; i < buildingCount; i++) {
        if(buildings[i].instanceId == id) {
            return &buildings[i];
        }
    }

    printf ("ERROR: no building of id %i", id);
    return NULL;
}



float GetBuildingToxicity (const Building* building) {

    float clumpToxicity = 3.0;
    float baseToxicity = building->data.toxicity;

    int clumpingTilesCounter;
    Tile* clumpingTiles = GetClumpingTiles (&gameState->grid, building->data.clumpArea, building->gridX, building->gridY, building->data.size, &clumpingTilesCounter, gameMemory);

    int clumpingBuildingCounter = 0;
    int* clumpingBuildingIds = (int*)gameMemory->temporaryStorageCurrent;

    for (int i = 0; i < clumpingTilesCounter; i++) {

        Building* nearbyBuilding = QueryBuilding(&clumpingTiles[i], gameState->session.buildings, gameState->session.buildingCounter);

        if(nearbyBuilding == NULL) continue;

        bool found = false;

        for (int j = 0; j < clumpingBuildingCounter; j++) {
            if(nearbyBuilding->instanceId == clumpingBuildingIds[j]) {
                found = true;
                break;
            }
        }  

        if(!found) {
            *(clumpingBuildingIds + clumpingBuildingCounter) = nearbyBuilding->instanceId;
            clumpingBuildingCounter ++;
            gameMemory->temporaryStorageCurrent += sizeof (int*);
        }

    } 

    return baseToxicity + (clumpingBuildingCounter * clumpToxicity);
}


// Sorts buildings for drawing. Writes on a copy buffer so as to
// not break all the goddam pointers to buildings
void SortBuildings (const Building* buildings, Building* sortedBuildings, u32 buildingCounter) {

    // copy
    for (int i = 0; i < buildingCounter; i++) {
        sortedBuildings[i] = buildings[i];
    }

    Building temp;
    // bubble sort
    for (int i = 0; i < buildingCounter; i++) {

        for (int j = 0; j < buildingCounter - i - 1; j++) {

            Building a = sortedBuildings [j];
            Building b = sortedBuildings [j+1];

            //@TODO fix, should check the difference so it wont cancel out
            if(a.gridY + a.data.size > b.gridY + b.data.size || 
              a.gridX - a.data.size < b.gridX - b.data.size) {

                temp = b;
                sortedBuildings[j+1] = a;
                sortedBuildings[j] = temp;
            }
        }
    }
}

Building CreateBuilding (BuildingType type, int gridX, int gridY) {
    
    Building building;
    building.instanceId = gameState->nextBuildingInstanceId;
    gameState->nextBuildingInstanceId += 1;
    building.gridX = gridX;
    building.gridY = gridY;
    building.data = GetBuildingData(type);
    return building;
}

BuildingData GetBuildingData (BuildingType type) {
    
    BuildingData data;

    switch (type)
    {
        // HOUSING 0
        case BUILDING_BLOCKS: {
            data = BuildingData {
                "Building Blocks",
                20,  // toxicity
                500000, // housing
                Resources {
                    -40, // energy
                    -50, // production
                    -40  // exchange
                },
                4,  //clumping
                2,  //size
                Color {100, 100, 100, 200},
                80, // height
                SUBURBAN_HOUSES // evolution
            };
        } break;
        
        // HOUSING 1
        case SUBURBAN_HOUSES: {
            data = BuildingData {
                "Suburban Houses",
                12,  // toxicity
                200000, // housing
                Resources {
                    -32, // energy
                    -42, // production
                    -32  // exchange
                },
                3,  //clumping
                2,  //size
                Color {80, 130, 100, 200},
                70, // height
                COMMUNAL_HOUSING // evolution
            };
        } break;

        // HOUSING 2
        case COMMUNAL_HOUSING : {
            data = BuildingData {
                "Communal Housing",
                8,  // toxicity
                100000, // housing
                Resources {
                    -22, // energy
                    -35, // production
                    -22  // exchange
                },
                3,  //clumping
                2,  //size
                Color {80, 150, 100, 200},
                60, // height
                TECHNO_KAUS // evolution
            };
        } break;

        // HOUSING 3
        case TECHNO_KAUS : {
            data = BuildingData {
                "Techno-Kaus",
                3,  // toxicity
                500000, // housing
                Resources {
                    -18, // energy
                    -25, // production
                    -18  // exchange
                },
                3,  //clumping
                2,  //size
                Color {100, 200, 100, 220},
                50 // height
            };
        } break;

        // ENERGY 0
        case COAL_PLANT: {
            data = BuildingData {
                "Coal Power Plant",
                40,  // toxicity
                0, // housing
                Resources {
                    500, // energy
                    -60, // production
                    -40  // exchange
                },
                3,  //clumping
                3, //size
                Color {130, 100, 100, 220},
                80, // height
                NATURAL_GAS,
            };
        } break;

        // ENERGY 1
        case NATURAL_GAS: {
            data = BuildingData {
                "Natural Gas Extractor",
                30,  // toxicity
                0, // housing
                Resources {
                    400, // energy
                    -50, // production
                    -30  // exchange
                },
                3,  //clumping
                3, //size
                Color {150, 100, 100, 220},
                70, // height
                NUCLEAR_PLANT
            };
        } break;

        // ENERGY 2
        case NUCLEAR_PLANT: {
            data = BuildingData {
                "Nuclear Fission Plant",
                20,  // toxicity
                0, // housing
                Resources {
                    300, // energy
                    -40, // production
                    -25  // exchange
                },
                3,  //clumping
                3, //size
                Color {170, 100, 90, 220},
                60, // height
                SOLAR_AND_WIND
            };
        } break;

        // ENERGY 3
        case SOLAR_AND_WIND: {
            data = BuildingData {
                "Solar and Wind",
                8,  // toxicity
                0, // housing
                Resources {
                    150, // energy
                    -20, // production
                    -12  // exchange
                },
                3,  //clumping
                3, //size
                Color {130, 80, 130, 220},
                40 // height
            };
        } break;

        // PRODUCTION 0
        case INDUSTRIAL_REFINERY: {
            data = BuildingData {
                "Factories and Refineries",
                30,  // toxicity
                0, // housing
                Resources {
                    -80, // energy
                    300, // production
                    -60  // exchange
                },
                3,  //clumping
                3, //size
                Color { 110, 130, 110, 220},
                70 // height
            };
         } break;



        // EXCHANGE 1
        case BUSINESS_OFFICES : {
            data = BuildingData {
                "Business Towers",
                30,  // toxicity
                0, // housing
                Resources {
                    -70, // energy
                    -40, // production
                    300  // exchange
                },
                3,  //clumping
                2, // size
                Color { 150, 150, 150, 220},
                90 // height
            };
        } break;

        data.type = type;

        return data;
    }
}