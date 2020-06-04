#ifndef H_AGORA
#define H_AGORA

struct WindowDimension {
    int width;
    int height;
};

struct GameMemory {
    bool isInitialized = false;
    u64 permanentStorageSize;
    void* permanentStorage;

    u64 temporaryStorageSize;
    void* temporaryStorage;

    void* temporaryStorageCurrent;
};

enum GamePhase {
    GP_IDLE,
    GP_MOVING_BUILDING,
};

#include "physics.h"
#include "graphics.h"
#include "sprites.h"
#include "text.h"
#include "resources.h"
#include "grid.h"
#include "buildings.h"
#include "simulation.h"
#include "ui.h"
#include <math.h> 

//Better version
struct GameButtonState {
    int halfTransitionCount;
    bool endedDown;
};

struct GameControllerInput {
    bool isAnalog;

    f32 startX, startY;
    f32 minX, minY;
    f32 maxX, maxY;
    f32 endX, endY;

    union {
        GameButtonState buttons [6];
        struct {
            GameButtonState up;
            GameButtonState down;
            GameButtonState left;
            GameButtonState right;
            GameButtonState leftShoulder;
            GameButtonState rightShoulder;
        };
    };
};

struct GameInput {
    GameControllerInput controllers [4];
};

struct Camera {
    float actualZoom;
    float targetZoom;
    Vector2 targetTranslation;
    Vector2 actualTranslation;
};

//Static buffers for most memory structs
struct GameState {

    SpriteSheet spriteSheets [256];
    u32 nextSpriteSheet = 0;

    Sprite _sprites [256];
    u32 nextSprite = 0;

    SpriteAnimation spriteAnimations [256];
    u32 nextSpriteAnimation;

    Text texts [256];
    u32 nextText = 0;

    UIButton* buttons;
    u32 buttonCounter;

    UIButton* hoverButton;

    // fonts?
    TTF_Font* mainFont;

    Sprite* backgroundSprite;
    Sprite* demolishIcon;
    Sprite* upgradeIcon;
    Sprite* moveIcon;
    
    Camera camera;

    TileGrid grid;
    int selectedBuildingId;
    int nextBuildingInstanceId;

    GameSession session;
};

GameState* gameState;

void GameInit(SDL_Surface * surface, GameMemory * gameMemory);

void GameUpdateAndRender (GameMemory * gameMemory, WindowDimension windowDimension, f64 msElapsed, GameInput input) ;

void GameCleanup(GameMemory * gm);

void RenderGrid();

Vector2 WorldToScreenPosition (Vector2 worldPosition);

void RegisterButton (UIButton button);

#endif  /* H_AGORA */
