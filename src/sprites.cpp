#include "sprites.h"

#include <GL/gl.h>
#include <SDL_opengl.h>

SpriteSheet * LoadSpriteSheet(char* filepath, GLenum type, ImageFormat format, SDL_Surface * windowSurface,
     int xCount, int yCount) {

    SpriteSheet * sheet = new SpriteSheet;

    /* Create storage space for the texture */
    SDL_Surface * textureImage[1];

    if(format == BMP) {
        textureImage[0] = SDL_LoadBMP(filepath);
    } else if (format == PNG) {
        textureImage[0] = LoadPNG(filepath, windowSurface);
    }

    if(textureImage[0] == NULL) {
        printf("failed to load texture at %s", filepath);
    }

    glGenTextures( 1, &sheet->texture );

    glBindTexture(GL_TEXTURE_2D, sheet->texture);

    sheet->width  = textureImage[0]->w;
    sheet->height = textureImage[0]->h;
    sheet->xCount = xCount;
    sheet->yCount = yCount;

    sheet->frameWidth  = sheet->width / sheet->xCount;
    sheet->frameHeight = sheet->height / sheet->yCount;

    glTexImage2D(GL_TEXTURE_2D, 0, 3, sheet->width,
        //need alpha channel support or something yeahhh
          sheet->height, 0, type,
          GL_UNSIGNED_BYTE, textureImage[0]->pixels);

    //TODO error handling
    glGetError();

    /* Linear Filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    if (textureImage[0]) {
        SDL_FreeSurface(textureImage[0]);
    }

    return sheet;
}

SpriteAnimation * LoadSpriteAnimation (SpriteSheet * spriteSheet,
                                        char* identifier,
                                        int frameDuration,
                                        bool isLoop,
                                        int xStart, int xEnd,
                                        int yStart, int yEnd) {

    SpriteAnimation * animation = new SpriteAnimation;

    animation->identifier = identifier;
    animation->isLoop    = isLoop;
    animation->frameDuration = frameDuration;
    animation->frameDurationCounter = 0;
    animation->currentFrame = 0;
    animation->spriteSheet = spriteSheet;

    animation->frameCount = 0;
    for(int y = yStart; y < yEnd; ++y) {
        for(int x = xStart; x < xEnd; ++x) {
            animation->frameCount++;
        }
    }

    if(animation->frameCount > MAX_FRAMES) {
        printf("You are trying to allocate a sprite animation with more than the maximum frames allowed");
    }

    int frame = 0;

    //TODO end is not inclusive and beginning is inclusive
    //super ugly, make better
    for(int y = yStart; y < yEnd; ++y) {
        for(int x = xStart; x < xEnd; ++x) {

            animation->frames[frame].xIndex = x;
            animation->frames[frame].yIndex = y;
            ++frame;
        }
    }

    return animation;
}

void UnloadSpriteAnimation (SpriteAnimation * spriteAnimation) {
    delete [] spriteAnimation->frames;
    delete spriteAnimation;
}


void RenderSpriteAnimation (SpriteAnimation * spriteAnimation,
                                float x, float y,
                                int screenWidth, int screenHeight,
                                int msElapsed, float scale = 1.f,
                                bool isFlipped = false) {

    //moves the counter and moves the current frame if needed
    spriteAnimation->frameDurationCounter += msElapsed;

    while(spriteAnimation->frameDurationCounter >= spriteAnimation->frameDuration) {

        spriteAnimation->currentFrame++;

        //reached the end of the animation
        if(spriteAnimation->currentFrame == spriteAnimation->frameCount) {
            if(spriteAnimation->isLoop) {
                spriteAnimation->currentFrame = 0;
            } else {
                spriteAnimation->currentFrame--;
            }
        }

        // moves the counter back
        spriteAnimation->frameDurationCounter -= spriteAnimation->frameDuration;
    }

    const SpriteSheet * spriteSheet = spriteAnimation->spriteSheet;
    const SpriteSheetFrame currentFrame = spriteAnimation->frames[spriteAnimation->currentFrame];

    float relativeWidthUnit = 1.f / spriteSheet->xCount;
    float relativeHeightUnit = 1.f / spriteSheet->yCount;

    float spriteWidth = scale * spriteSheet->width / spriteSheet->xCount / (float)screenWidth;
    float spriteHeight = scale * spriteSheet->height / spriteSheet->yCount / (float)screenHeight;

    //centered around insertion point
    float xPosition = x - (spriteWidth / 2);
    float yPosition = y - (spriteHeight / 2);

    //bind texture
    glBindTexture(GL_TEXTURE_2D, spriteSheet->texture);

    glBegin(GL_QUADS);
        //tint
        glColor4d(1, 1, 1, 1);

        //OpenGL
        //top left
        glTexCoord2f(relativeWidthUnit * currentFrame.xIndex,          //x
                    relativeHeightUnit * (currentFrame.yIndex + 1));   //y
        if(isFlipped) {
            glVertex2f( xPosition + spriteWidth, yPosition);
        } else {
            glVertex2f( xPosition, yPosition);
        }
        //top right
        glTexCoord2f(relativeWidthUnit * (currentFrame.xIndex + 1),    //x
                    relativeHeightUnit * (currentFrame.yIndex + 1));   //y
        if(isFlipped) {
            glVertex2f( xPosition, yPosition);
        } else {
            glVertex2f( xPosition + spriteWidth, yPosition);
        }

        //bot right
        glTexCoord2f(relativeWidthUnit * (currentFrame.xIndex + 1),    //x
                    relativeHeightUnit * currentFrame.yIndex);         //y
        if(isFlipped) {
            glVertex2f( xPosition, yPosition + spriteHeight);
        } else {
            glVertex2f( xPosition + spriteWidth, yPosition + spriteHeight);
        }
        //bot left
        glTexCoord2f(relativeWidthUnit * currentFrame.xIndex,          //x
                    relativeHeightUnit * currentFrame.yIndex);         //y
        if(isFlipped) {
            glVertex2f( xPosition + spriteWidth, yPosition + spriteHeight);
        } else {
            glVertex2f( xPosition, yPosition + spriteHeight);
        }
    glEnd();
}

void ResetSpriteAnimation (SpriteAnimation * spriteAnimation) {
    spriteAnimation->currentFrame = 0;
    spriteAnimation->frameDurationCounter = 0;
}

SDL_Surface * LoadPNG (char* filepath, SDL_Surface * windowSurface) {

    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load (filepath);

    if(loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", filepath, IMG_GetError() );
        return NULL;
    }

    //Convert surface to screen format
    optimizedSurface = SDL_ConvertSurface(loadedSurface, windowSurface->format, NULL);

    if(optimizedSurface == NULL) {
        printf( "Unable to optimize image %s! SDL Error: %s\n", filepath, SDL_GetError() );
        return NULL;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface( loadedSurface );

    return optimizedSurface;
}
