#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL_opengl.h>

#include "sprites.cpp"
#include "crimson_spiral.h"


//TODO find some nice place to store this!
SpriteSheet * runningDude;
SpriteSheet * otherDude;
SpriteSheet * someGirl;

SpriteAnimation * girlMoveRight;

SpriteAnimation * runningDudeAnimation1;
SpriteAnimation * runningDudeAnimation2;

SpriteAnimation * fighterMoveRight;
SpriteAnimation * fighterMoveLeft;
SpriteAnimation * fighterMoveUp;
SpriteAnimation * fighterMoveDown;

Fighter * fighter;

void GameInit(SDL_Surface * windowSurface) {

    runningDude = LoadSpriteSheet("../assets/spritesheets/test_spritesheet.bmp",
                                    GL_BGR, BMP, windowSurface, 6, 5);
    otherDude = LoadSpriteSheet("../assets/spritesheets/other_dude.bmp",
                                    GL_BGRA, BMP, windowSurface, 9, 4);

    someGirl = LoadSpriteSheet("../assets/spritesheets/some_girl.png",
                                GL_BGRA, PNG, windowSurface, 14, 4);


    girlMoveRight       = LoadSpriteAnimation (someGirl, "someGirlR", 64, true, 0, 14, 2, 3);

    fighterMoveRight    = LoadSpriteAnimation (otherDude, "otherDude", 64, true, 0, 9, 3, 4);
    fighterMoveLeft     = LoadSpriteAnimation (otherDude, "otherDude", 64, true, 0, 9, 1, 2);
    fighterMoveUp       = LoadSpriteAnimation (otherDude, "otherDude", 64, true, 0, 9, 0, 1);
    fighterMoveDown     = LoadSpriteAnimation (otherDude, "otherDude", 64, true, 0, 9, 2, 3);

    runningDudeAnimation1 = LoadSpriteAnimation(runningDude, "runningDude1", 16, true, 0, 6, 0, 5);
    runningDudeAnimation2 = LoadSpriteAnimation(runningDude, "runningDude2", 32, true, 0, 6, 0, 5);
    audioTest.Initialize();

    fighter = new Fighter();

    fighter->moveUp     = fighterMoveUp;
    fighter->moveDown   = fighterMoveDown;
    fighter->moveLeft   = fighterMoveLeft;
    fighter->moveRight  = fighterMoveRight;

    fighter->position.x = 0.f;
    fighter->position.y = 0.f;
    fighter->position.z = 0.f;
    fighter->scale = 3.f;
}

void GameUpdateAndRender (int screenWidth, int screenHeight, f64 msElapsed, TempGameInput input) {

    GLenum error = GL_NO_ERROR;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSpriteAnimation(runningDudeAnimation1, 0.5f, 0.5f, screenWidth, screenHeight, msElapsed);
    RenderSpriteAnimation(runningDudeAnimation2, -0.5f, -0.5f, screenWidth, screenHeight, msElapsed, 1.5f, true);
    RenderSpriteAnimation(girlMoveRight, -0.5f, 0.5f, screenWidth, screenHeight, msElapsed, 3.f, false);

    UpdateAndRenderFighter(fighter, &input, screenWidth, screenHeight, msElapsed);

    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("OpenGL Error: %s\n", gluErrorString( error ));
    }

    //game_output_sound(sound_buffer, tone_hz);
}

void UpdateAndRenderFighter (Fighter * fighter, TempGameInput * input, int screenWidth, int screenHeight, f64 msElapsed) {

    float deltaTime = msElapsed / 1000.0f;
    static const float fighterSpeed = 0.3f;

    SpriteAnimation * animation = fighter->moveRight;

    // move the fighter in the horizontal axis.
    fighter->position.x += fighterSpeed * deltaTime * input->xAxis;
    fighter->position.y += fighterSpeed * deltaTime * input->yAxis;

    if(input->xAxis > 0) animation = fighter->moveRight;
    if(input->xAxis < 0) animation = fighter->moveLeft;
    if(input->yAxis > 0) animation = fighter->moveUp;
    if(input->yAxis < 0) animation = fighter->moveDown;

    RenderSpriteAnimation(animation, fighter->position.x, fighter->position.y,
                        screenWidth, screenHeight, msElapsed, fighter->scale, false);
}

void GameCleanup() {
    UnloadSpriteAnimation(runningDudeAnimation1);
    UnloadSpriteAnimation(runningDudeAnimation2);
    UnloadSpriteAnimation(fighterMoveUp);
    UnloadSpriteAnimation(fighterMoveLeft);
    UnloadSpriteAnimation(fighterMoveRight);
    UnloadSpriteAnimation(fighterMoveDown);
    //TODO unload sprite sheets

    delete fighter;
}
