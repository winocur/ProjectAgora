#ifndef H_GRID
#define H_GRID

enum TileState {
    FREE,
    OCCUPIED
};

struct Tile {
    Vector2 center;

    BoundingBox box;

    int indexX;
    int indexY;
};

struct TileGrid {

    int height;
    int width;

    Tile tiles [64][64] ;

    float tileHeight;
    float tileWidth;

    float squash;

    Vector2 center = { 0, 0 };

    int selectedX, selectedY;
};

TileGrid* grid;

void PopulateGrid (TileGrid * grid);

void RenderGrid (TileGrid* grid);

Tile QueryTile (TileGrid* grid, Vector2 worldPosition);

void HighlightTile (Tile tile, Color color);

Vector2 ScreenToProjection (Vector2 screenPoint, float squash = 0.5f);

Vector2 ProjectionToScreen (Vector2 projectedPoint, float squash = 0.5f);

Tile* GetClumpingTiles (TileGrid* grid, int clumpArea, int centerX, int centerY, int objectSize, int* outTiles, GameMemory* gameMemory);

#endif