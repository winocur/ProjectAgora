#include "crimson_spiral.h"

#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL_opengl.h>

#include "sprites.cpp"

//TODO find some nice place to store this!
SpriteSheet * runningDude;
SpriteSheet * otherDude;
SpriteAnimation * runningDudeAnimation1;
SpriteAnimation * runningDudeAnimation2;
SpriteAnimation * otherDudeAnimation;


void GameInit() {

    runningDude = LoadSpriteSheet("../assets/test_spritesheet.bmp", GL_BGR, 6, 5);
    otherDude = LoadSpriteSheet("../assets/other_dude.bmp", GL_BGRA, 9, 4);
    otherDudeAnimation = LoadSpriteAnimation (otherDude, "otherDude", 64, true, 0, 9, 2, 3);
    runningDudeAnimation1 = LoadSpriteAnimation(runningDude, "runningDude1", 16, true, 0, 6, 0, 5);
    runningDudeAnimation2 = LoadSpriteAnimation(runningDude, "runningDude2", 32, true, 0, 6, 0, 5);
}

void GameUpdateAndRender (int screenWidth, int screenHeight, int msElapsed) {

    GLenum error = GL_NO_ERROR;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    RenderSpriteAnimation(runningDudeAnimation1, 0.5f, 0.5f, screenWidth, screenHeight, msElapsed);
    RenderSpriteAnimation(runningDudeAnimation2, -0.5f, -0.5f, screenWidth, screenHeight, msElapsed, 1.5f, true);
    RenderSpriteAnimation(otherDudeAnimation, -0.5f, 0.5f, screenWidth, screenHeight, msElapsed, 3.f, false);

    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("OpenGL Error: %s\n", gluErrorString( error ));
    }

    //game_output_sound(sound_buffer, tone_hz);
}

void GameCleanup() {
    UnloadSpriteAnimation(runningDudeAnimation1);
    UnloadSpriteAnimation(runningDudeAnimation2);
    UnloadSpriteAnimation(otherDudeAnimation);
    //TODO unload sprite sheets
}
