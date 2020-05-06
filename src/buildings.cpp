#include "buildings.h"


void RenderBuilding (const Building* building, const TileGrid* grid) {

    Tile tile = grid->tiles[building->gridX][building->gridY];

    int roof = building->roofHeight;

    Vector2 bottomLeft = ScreenToProjection( { tile.center.x - grid->tileWidth / 2 - building->size * grid->tileWidth, tile.center.y - grid->tileHeight / 2 - building->size * grid->tileHeight });
    Vector2 topLeft = ScreenToProjection( { tile.center.x - grid->tileWidth / 2 - building->size * grid->tileWidth, tile.center.y + grid->tileHeight / 2 + building->size * grid->tileHeight });
    Vector2 topRight = ScreenToProjection( { tile.center.x + grid->tileWidth / 2 + building->size * grid->tileWidth, tile.center.y + grid->tileHeight / 2 + building->size * grid->tileHeight });
    Vector2 bottomRight = ScreenToProjection( { tile.center.x + grid->tileWidth / 2 + building->size * grid->tileWidth, tile.center.y - grid->tileHeight / 2 - building->size * grid->tileHeight });

    Color baseColor = building->color;
    Color midColor = baseColor + Color { - 50, -50, -50, 0};
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

Vector2 GetPosition (const Building* building, const TileGrid* grid) {

    Tile tile = grid->tiles[building->gridX][building->gridY];
    return Vector2 { tile.center.x, tile.center.y };
}

Building* QueryBuilding (const Tile* tile, Building* buildings, const u32 buildingCount) {

    for (int i = 0; i < buildingCount; i++) {

        Building* building = buildings + i;

        if(building->state == DESTROYED) continue;
        
        int size = building->size;

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
            if(a.gridY + a.size > b.gridY + b.size || 
              a.gridX - a.size < b.gridX - b.size) {

                temp = b;
                sortedBuildings[j+1] = a;
                sortedBuildings[j] = temp;
            }
        }
    }
}

Building CreateBuilding (int classId, int size, Color color, int gridX, int gridY, int roofHeight) {
    
    Building building;
    building.classId = classId;
    building.instanceId = gameState->nextBuildingInstanceId;
    gameState->nextBuildingInstanceId += 1;
    building.size = size;
    building.color = color;
    building.gridX = gridX;
    building.gridY = gridY;
    building.roofHeight = roofHeight;
    building.data = GetBuildingData(classId);
    return building;
}

BuildingData GetBuildingData (int classId) {
    
    switch (classId)
    {
        // HOUSING 1
        case 1: {
            return BuildingData {
                "Building Blocks",
                20,  // toxicity
                500000, // housing
                Resources {
                    -40, // energy
                    -50, // production
                    -40  // exchange
                },
                3,  //clumping
                classId,
                5 // evolves into
            };
        } break;
        
        // ENERGY 1
        case 2: {
            return BuildingData {
                "Coal Power Plant",
                40,  // toxicity
                0, // housing
                Resources {
                    500, // energy
                    -60, // production
                    -40  // exchange
                },
                3,  //clumping
                classId,
                6 // evolves into
            };
        } break;

        // PRODUCTION 1
        case 3: {
            return BuildingData {
                "Factories and Refineries",
                30,  // toxicity
                0, // housing
                Resources {
                    -80, // energy
                    300, // production
                    -60  // exchange
                },
                3,  //clumping
                classId,
                7 // evolves into
            };
         } break;

        // EXCHANGE 1
        case 4: {
            return BuildingData {
                "Business Towers",
                30,  // toxicity
                0, // housing
                Resources {
                    -70, // energy
                    -40, // production
                    300  // exchange
                },
                3,  //clumping
                classId,
                8 // evolves into
            };
        } break;
    }
}