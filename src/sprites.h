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

struct Sprite {
    char* identifier;
    SpriteSheet* spriteSheet;

    int xIndex, yIndex;
};

struct SpriteAnimation {
    Sprite frames [MAX_FRAMES];
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

SpriteSheet *
LoadSpriteSheet(SpriteSheet * sheet, char* filepath,
                    GLenum type, ImageFormat format,
                    SDL_Surface * windowSurface,
                    int xCount, int yCount);

SpriteAnimation *
LoadSpriteAnimation ( SpriteAnimation * animation,
                           const SpriteSheet * spriteSheet,
                           char* identifier,
                           int frameDuration,
                           bool isLoop,
                           int xStart, int xEnd,
                           int yStart, int yEnd);

Sprite *
LoadSprite (Sprite * sprite,
                const SpriteSheet * spriteSheet,
                char* identifier,
                int xIndex , int yIndex);

void RenderSpriteAnimation (const SpriteAnimation * spriteAnimation,
                                int msElapsed,
                                float x, float y, float z,
                                float scale,
                                bool isFlipped);

void RenderSprite (const Sprite * sprite,
                    float x, float y, float z,
                    float scale,
                    bool isFlipped);

void RenderSpriteToBox (const Sprite* sprite, 
                        BoundingBox box, 
                        bool isFlipped = false);

void ResetSpriteAnimation (SpriteAnimation * spriteAnimation);


SDL_Surface * LoadPNG (char* filepath, SDL_Surface * windowSurface) ;


#endif
