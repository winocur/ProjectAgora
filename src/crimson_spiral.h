#ifndef H_CRIMSONSPIRAL
#define H_CRIMSONSPIRAL

#include "sprites.h"
// Audio
#include "Sonos.h"
Sonos audioTest = { 0, 0, 0, 0, 0, {} };
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
    SpriteAnimation* lastAnimation;
    f32 scale;

    bool idle = true;
    i32 idleCounter = 200;
};

void GameInit(SDL_Surface * surface);

void GameUpdateAndRender (int screenWidth, int screenHeight, f64 msElapsed, TempGameInput input) ;
void UpdateAndRenderFighter (Fighter * fighter, TempGameInput * input, int screenWidth, int screenHeight, f64 msElapsed);

void GameCleanup();

#endif  /* H_CRIMSONSPIRAL */
