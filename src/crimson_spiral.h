#ifndef crimsonSpiral
#define crimsonSpiral

#include "sprites.h"

//input

//Temporary for test purpouses
struct TempGameInput {
    f32 xAxis;
    f32 yAxis;
};

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

struct Vector3 {
    f64 x, y, z;
};

struct Fighter {
    Vector3 position;

    SpriteAnimation* moveRight;
    SpriteAnimation* moveLeft;
    SpriteAnimation* moveUp;
    SpriteAnimation* moveDown;

    f32 scale;
};

void GameInit();

void GameUpdateAndRender (int screenWidth, int screenHeight, f64 msElapsed, TempGameInput input) ;

void GameCleanup();

void UpdateAndRenderFighter (Fighter * fighter, TempGameInput * input, int screenWidth, int screenHeight, f64 msElapsed);



#endif
