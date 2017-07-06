#ifndef sprites
#define sprites

#include <GL/glu.h>

#define MAX_FRAMES 256

enum ImageFormat {
    BMP,
    PNG
};

struct SpriteSheet {
    int width;
    int height;
    int xCount;
    int yCount;
    int frameWidth;
    int frameHeight;
    GLuint texture;
};

struct SpriteSheetFrame {
    int xIndex;
    int yIndex;
};

struct SpriteAnimation {
    SpriteSheetFrame frames [MAX_FRAMES];
    int frameCount;

    char* identifier;
    SpriteSheet* spriteSheet;

    bool isLoop;
    int currentFrame;
    int frameDuration;
    int frameDurationCounter;

    float relativeWidthUnit;
    float relativeHeightUnit;
};

struct Sprite {
    char* identifier;
    SpriteSheet* spriteSheet;

    int xIndex, yIndex;

    float relativeWidthUnit;
    float relativeHeightUnit;
};

SpriteSheet * LoadSpriteSheet (char* filepath, GLenum type, ImageFormat format, SDL_Surface * windowSurface, int xCount, int yCount) ;

SpriteAnimation * LoadSpriteAnimation (SpriteSheet * spriteSheet,
                                        char* identifier,
                                        int frameDuration,
                                        bool isLoop,
                                        int xStart, int xEnd,
                                        int yStart, int yEnd
                                        );

Sprite * LoadSprite (SpriteSheet * spriteSheet, char* identifier, int xSheet , int ySheet);

void UnloadSprite (Sprite * sprite);

void UnloadSpriteAnimation (SpriteAnimation * spriteAnimation);

void RenderSpriteAnimation (SpriteAnimation * spriteAnimation,
                                float x, float y,
                                int screenWidth, int screenHeight,
                                int msElapsed, float scale,
                                bool isFlipped);

void RenderSprite (Sprite * sprite,
                    float x, float y,
                    int screenWidth, int screenHeight, float scale,
                    bool isFlipped);


void ResetSpriteAnimation (SpriteAnimation * spriteAnimation);

SDL_Surface * LoadPNG (char* filepath, SDL_Surface * windowSurface) ;


#endif
