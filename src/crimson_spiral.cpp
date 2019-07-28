#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL_opengl.h>

#include "crimson_spiral.h"

#include "sprites.cpp"


void GameInit(SDL_Surface * windowSurface, GameMemory * gameMemory) {

    //initialize game memory
    GameState * gs = (GameState *)gameMemory->permanentStorage;

    gs->runningDude = LoadSpriteSheet(GetNextSpriteSheet(gs), "../assets/spritesheets/test_spritesheet.bmp",
                                    GL_BGR, BMP, windowSurface, 6, 5);

    gs->otherDude   = LoadSpriteSheet(GetNextSpriteSheet(gs), "../assets/spritesheets/other_dude.bmp",
                                    GL_BGRA, BMP, windowSurface, 9, 4);

    gs->someGirl    = LoadSpriteSheet(GetNextSpriteSheet(gs), "../assets/spritesheets/some_girl.png",
                                    GL_RGBA, PNG, windowSurface, 14, 4);

    gs->backgroundSheet = LoadSpriteSheet (GetNextSpriteSheet(gs), "../assets/images/test_bg.png",
                                        GL_RGB, PNG, windowSurface, 1, 1);

    gs->barrelSheet = LoadSpriteSheet (GetNextSpriteSheet(gs), "../assets/images/barrel_1.png",
                                        GL_RGBA, PNG, windowSurface, 1, 1);



    gs->girlMoveRight = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->someGirl, "someGirl", 64, true, 0, 14, 2, 3);

    gs->runningDudeAnimation1 = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->runningDude, "runningDude1", 16, true, 0, 6, 0, 5);

    gs->runningDudeAnimation2 = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->runningDude, "runningDude2", 32, true, 0, 6, 0, 5);

    gs->backgroundSprite = LoadSprite(GetNextSprite(gs), gs->backgroundSheet, "background", 0, 0);

    gs->barrel = LoadSprite(GetNextSprite(gs), gs->barrelSheet, "barrel", 0, 0);

    Fighter * fighter = &gs->fighter;

    fighter->moveUp     = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->otherDude, "otherDude", 64, true, 0, 9, 0, 1);
    fighter->moveDown   = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->otherDude, "otherDude", 64, true, 0, 9, 2, 3);
    fighter->moveLeft   = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->otherDude, "otherDude", 64, true, 0, 9, 1, 2);
    fighter->moveRight  = LoadSpriteAnimation (GetNextSpriteAnimation(gs), gs->otherDude, "otherDude", 64, true, 0, 9, 3, 4);
    fighter->lastAnimation = fighter->moveRight;

    fighter->position.x = 100.f;
    fighter->position.y = 100.f;
    fighter->position.z = 0.f;
    fighter->scale = 2.f;
}


void GameUpdateAndRender (GameMemory * gameMemory,
                        int screenWidth, int screenHeight,
                        f64 msElapsed, TempGameInput input) {

    GLenum error = GL_NO_ERROR;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Pop default matrix onto current matrix
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    //Move camera to position
    glTranslatef(input.xCamera, input.yCamera , 0);

    //Save default matrix again with camera translation
    glPushMatrix();

    GameState * gs = (GameState *)gameMemory->permanentStorage;

    RenderSprite(gs->backgroundSprite, screenWidth / 2, (screenHeight / 2), -300, 1.5f);

    //throw in some barrels

    RenderSprite(gs->barrel, 100, -20, -300, 0.20);
    RenderSprite(gs->barrel, 160, -20, -300, 0.20);
    RenderSprite(gs->barrel, 230, -40, -200, 0.20);

    UpdateAndRenderFighter(&gs->fighter, &input, msElapsed);

    RenderSprite(gs->barrel, 500, 70, 150, 0.20);
    RenderSprite(gs->barrel, 600, 70, 150, 0.20);
    RenderSprite(gs->barrel, 700, 70, 150, 0.20);


    error = glGetError();
    if(error != GL_NO_ERROR) {
        printf("OpenGL Error: %s\n", gluErrorString( error ));
    }

}

void UpdateAndRenderFighter (Fighter * fighter, TempGameInput * input, f64 msElapsed) {

    float deltaTime = msElapsed / 1000.0f;
    static const float fighterSpeed = 160.f;

    SpriteAnimation * animation = fighter->moveRight;

    // move the fighter in the horizontal axis.
    fighter->position.x += fighterSpeed * deltaTime * input->xAxis;
    fighter->position.y += fighterSpeed * deltaTime * input->yAxis;

    //TODO make idle checks better
    if(input->xAxis > 0) {
        animation = fighter->moveRight;
        fighter->idleCounter = 200;
    } else if(input->xAxis < 0) {
        animation = fighter->moveLeft;
        fighter->idleCounter = 200;
    } else if(input->yAxis > 0) {
        animation = fighter->moveUp;
        fighter->idleCounter = 200;
    } else if(input->yAxis < 0) {
        animation = fighter->moveDown;
        fighter->idleCounter = 200;
    } else {
        fighter->idleCounter -= msElapsed;
        animation = fighter->lastAnimation;

        if(fighter->idleCounter < 0) {
            ResetSpriteAnimation(animation);
        }

        msElapsed = 0;
    }

    RenderSpriteAnimation(animation, msElapsed, fighter->position.x, fighter->position.y, 0.f, fighter->scale, false);

    fighter->lastAnimation = animation;
}

void GameCleanup(GameMemory * gm) {
    free (gm->permanentStorage);
    free (gm->temporaryStorage);
}

Sprite * GetNextSprite (GameState * gs) {
    Sprite * sprite = gs->_sprites + gs->nextSprite;
    gs->nextSprite++;
    return sprite;
}

SpriteSheet * GetNextSpriteSheet (GameState * gs) {
    SpriteSheet * spriteSheet = gs->spriteSheets + gs->nextSpriteSheet;
    gs->nextSpriteSheet++;
    return spriteSheet;
}

SpriteAnimation * GetNextSpriteAnimation (GameState * gs) {
    SpriteAnimation * spriteAnimation = gs->spriteAnimations + gs->nextSpriteAnimation;
    gs->nextSpriteAnimation++;
    return spriteAnimation;
}
