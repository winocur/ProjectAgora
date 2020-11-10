#include "grid.h"

void PopulateGrid (TileGrid * grid) {

    grid->height = 16;
    grid->width = 16;

    grid->tileHeight = 30;
    grid->tileWidth = 30;

    grid->center = { 0,0 };

    grid->selectedX = 8;
    grid->selectedY = 8;

    for (int i = 0; i < grid->height; i++) {

        for (int j = 0; j < grid->width; j++) {

            float x = grid->center.x - (grid->tileWidth * (grid->width / 2.0)) + grid->tileWidth * i;
            float y = grid->center.y - (grid->tileHeight * (grid->height / 2.0)) + grid->tileHeight * j;
            
            BoundingBox box = { x - grid->tileWidth / 2.0, y - grid->tileHeight / 2.0, grid->tileWidth, grid->tileHeight};

            grid->tiles[i][j] = { { x, y }, box, i, j };
        }
    }

    grid->squash = grid->tileHeight / grid->tileWidth;
}

void RenderGrid (TileGrid* grid) {

    glColor4ub (210, 210, 210, 124);
    glLineWidth (3.0);

    float halfHeight = grid->tileHeight / 2;
    float halfWidth  = grid->tileWidth / 2;
    
    for (int i = 0; i < grid->width; i++) {

        for (int j = 0; j < grid->height; j++) {
            Tile tile = grid->tiles[i][j];

            Vector2 bottomLeft = ScreenToProjection( { tile.box.x, tile.box.y });

            Vector2 topLeft = ScreenToProjection( { tile.box.x, tile.box.y + tile.box.height });

            Vector2 topRight = ScreenToProjection( { tile.box.x + tile.box.width, tile.box.y + tile.box.height });

            Vector2 bottomRight = ScreenToProjection( { tile.box.x + tile.box.width, tile.box.y });
            
            glBegin (GL_LINE_STRIP);
            glVertex3f (bottomLeft.x, bottomLeft.y,  0);
            glVertex3f (topLeft.x, topLeft.y, 0);
            glVertex3f (topRight.x, topRight.y,  0);
            glVertex3f (bottomRight.x, bottomRight.y, 0);
            glVertex3f (bottomLeft.x, bottomLeft.y,  0);
            glEnd();
        }
    }

    HighlightTile(grid->tiles[grid->selectedX][grid->selectedY], {255, 255, 255, 150});
   
}

Tile QueryTile (TileGrid* grid, Vector2 worldPosition) {

    //printf("Query at [%f, %f]", worldPosition.x, worldPosition.y);

    float halfHeight = grid->tileHeight / 2;
    float halfWidth  = grid->tileWidth / 2;

    Vector2 screenPosition = ProjectionToScreen(worldPosition);

    for (int i = 0; i < grid->width; i++) {

        for (int j = 0; j < grid->height; j++) {
            
            Tile tile = grid->tiles[i][j]; 

            if(CheckCollision(screenPosition, tile.box)) {
                return tile;
            }

        }
    }

    //printf("Failed to find tile");
    return grid->tiles[0][0];

}

Tile* GetClumpingTiles (TileGrid* grid, int clumpArea, int centerX, int centerY, int objectSize, int* outTiles, GameMemory* gameMemory) {

    int clumpingSize = clumpArea;
    Tile* tiles = (Tile*)gameMemory->temporaryStorageCurrent;
    int clumpingCounter = 0;

    for (int i = 1; i <= clumpingSize; i++) {
        
        int sweep = objectSize + (clumpingSize - i);

        for (int j = -sweep; j <= sweep; j++) {
            int clumpingX = centerX + j;
            int clumpingY = centerY - objectSize - i;
            *(tiles + clumpingCounter) = grid->tiles[clumpingX][clumpingY];
            gameMemory->temporaryStorageCurrent += sizeof(Tile*);
            clumpingCounter++;
        }

        for (int j = -sweep; j <= sweep; j++) {
            int clumpingX = centerX + j;
            int clumpingY = centerY + objectSize + i;
            *(tiles + clumpingCounter) = grid->tiles[clumpingX][clumpingY];
            gameMemory->temporaryStorageCurrent += sizeof(Tile*);
            clumpingCounter++;
        }

        for (int j = -objectSize; j <= objectSize; j++) {
            int clumpingX = centerX - objectSize - i;
            int clumpingY = centerY + j;
            *(tiles + clumpingCounter) = grid->tiles[clumpingX][clumpingY];
            gameMemory->temporaryStorageCurrent += sizeof(Tile*);
            clumpingCounter++;
        }

        for (int j = -objectSize; j <= objectSize; j++) {
            int clumpingX = centerX + objectSize + i;
            int clumpingY = centerY + j;
            *(tiles + clumpingCounter) = grid->tiles[clumpingX][clumpingY];
            gameMemory->temporaryStorageCurrent += sizeof(Tile*);
            clumpingCounter++;
        }
        
    }
  
    *outTiles = clumpingCounter;
    return tiles;
}

void HighlightTile (Tile tile, Color color) {

    Vector2 bottomLeft = ScreenToProjection( { tile.box.x, tile.box.y });
    Vector2 topLeft = ScreenToProjection( { tile.box.x, tile.box.y + tile.box.height });
    Vector2 topRight = ScreenToProjection( { tile.box.x + tile.box.width, tile.box.y + tile.box.height });
    Vector2 bottomRight = ScreenToProjection( { tile.box.x + tile.box.width, tile.box.y });

    // selected 
    glColor4ub (color.r, color.g, color.b, color.a);

    glBegin (GL_QUADS);
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glVertex3f (topLeft.x, topLeft.y, 0);
    glVertex3f (topRight.x, topRight.y,  0);
    glVertex3f (bottomRight.x, bottomRight.y, 0);
    glVertex3f (bottomLeft.x, bottomLeft.y,  0);
    glEnd();
}

Vector2 ScreenToProjection (Vector2 screenPoint, float squash) {
    return { screenPoint.x + screenPoint.y , (screenPoint.x - screenPoint.y) * squash };
}

Vector2 ProjectionToScreen (Vector2 projectedPoint, float squash) {
    
    float x = (projectedPoint.x) / 2;
    float y = (projectedPoint.y) / 2 / squash;
 
    return { x + y, x - y };
}