#include "sprites.h"

#include <GL/gl.h>
#include <SDL_opengl.h>


SpriteSheet * LoadSpriteSheet(char* filepath, GLenum type, int xCount, int yCount) {
    SpriteSheet * sheet = new SpriteSheet;

    /* Create storage space for the texture */
    SDL_Surface * textureImage[1];

    if (textureImage[0] = SDL_LoadBMP(filepath)) {

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
    } else {
        printf("failed to load spritesheet");
    }

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



// -- test code -- //

//Texture storage.
GLuint texture[1];

int textureWidth;
int textureHeight;

void LoadSprite(char* path) {

    /* Create storage space for the texture */
    SDL_Surface *TextureImage[1];

    if (TextureImage[0] = SDL_LoadBMP(path)) {

        /* Create The Texture */
        glGenTextures( 1, &texture[0] );

        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture( GL_TEXTURE_2D, texture[0] );

        textureWidth =  TextureImage[0]->w;
        textureHeight = TextureImage[0]->h;
        /* Generate The Texture */
        glTexImage2D( GL_TEXTURE_2D, 0, 3, textureWidth,
              textureHeight , 0, GL_BGR,
              GL_UNSIGNED_BYTE, TextureImage[0]->pixels );
        glGetError();

        /* Linear Filtering */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    } else {
        printf("failed to load spritesheet");
    }

    /* Free up any memory we may have used */
    if ( TextureImage[0] ) {
        SDL_FreeSurface( TextureImage[0] );
    }

}

void RenderSprite (int screenWidth, int screenHeight) {

    float scale = 2;

    const float widthItemCount = 6;
    const float heightItemCount = 5;

    static float widthIndex = 0;
    static float heightIndex = 0;

    float relativeWidthUnit = 1.f / widthItemCount;
    float relativeHeightUnit = 1.f / heightItemCount;

    float relativeTextureWidth = scale * textureWidth / widthItemCount / (float)screenWidth;
    float relativeTextureHeight = scale * textureHeight / heightItemCount / (float)screenHeight;

    float xPosition = 0 - (relativeTextureWidth / 2);
    float yPosition = 0 - (relativeTextureHeight / 2);

    glBegin(GL_QUADS);
        //tinte
        glColor4d(1, 1, 1, 1);

        //OpenGL
        //top left
        glTexCoord2f(relativeWidthUnit * widthIndex, relativeHeightUnit * (heightIndex + 1));
        glVertex2f( xPosition, yPosition);

        //top right
        glTexCoord2f(relativeWidthUnit * (widthIndex + 1), relativeHeightUnit * (heightIndex + 1));
        glVertex2f( xPosition + relativeTextureWidth, yPosition);

        //bot right
        glTexCoord2f(relativeWidthUnit * (widthIndex + 1), relativeHeightUnit * heightIndex);
        glVertex2f( xPosition + relativeTextureWidth, yPosition + relativeTextureHeight);

        //bot left
        glTexCoord2f(relativeWidthUnit * widthIndex, relativeHeightUnit * heightIndex);
        glVertex2f( xPosition, yPosition + relativeTextureHeight);

    glEnd();

    //wraps around indexes
    widthIndex++;
    if(widthIndex == widthItemCount) {
        widthIndex = 0;
        heightIndex++;
        if(heightIndex == heightItemCount) {
            heightIndex = 0;
        }
    }

}
