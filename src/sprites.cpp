

#include "sprites.h"

#include <GL\GLU.h>
#include <GL\GL.h>
#include <SDL_opengl.h>

//Texture storage.
GLuint texture[1];

int texture_width;
int texture_height;

void load_sprite(char* path) {

    /* Create storage space for the texture */
    SDL_Surface *TextureImage[1];

    if (TextureImage[0] = SDL_LoadBMP(path)) {

        /* Create The Texture */
        glGenTextures( 1, &texture[0] );

        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture( GL_TEXTURE_2D, texture[0] );

        texture_width =  TextureImage[0]->w;
        texture_height = TextureImage[0]->h;
        /* Generate The Texture */
        glTexImage2D( GL_TEXTURE_2D, 0, 3, texture_width,
              texture_height , 0, GL_RGB,
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

void render_sprite (int screen_width, int screen_height) {

    float scale = 2;

    const float width_item_count = 6;
    const float height_item_count = 5;

    static float width_index = 0;
    static float height_index = 0;

    float relative_width_unit = 1.f / width_item_count;
    float relative_height_unit = 1.f / height_item_count;

    float relative_texture_width = scale * texture_width / width_item_count / (float)screen_width;
    float relative_texture_height = scale * texture_height / height_item_count / (float)screen_height;

    float x_position = 0 - (relative_texture_width / 2);
    float y_position = 0 - (relative_texture_height / 2);

    glBegin(GL_QUADS);
        //tinte
        glColor4d(1, 1, 1, 1);

        //OpenGL
        //top left
        glTexCoord2f(relative_width_unit * width_index, relative_height_unit * (height_index + 1));
        glVertex2f( x_position, y_position);

        //top right
        glTexCoord2f(relative_width_unit * (width_index + 1), relative_height_unit * (height_index + 1));
        glVertex2f( x_position + relative_texture_width, y_position);

        //bot right
        glTexCoord2f(relative_width_unit * (width_index + 1), relative_height_unit * height_index);
        glVertex2f( x_position + relative_texture_width, y_position + relative_texture_height);

        //bot left
        glTexCoord2f(relative_width_unit * width_index, relative_height_unit * height_index);
        glVertex2f( x_position, y_position + relative_texture_height);

    glEnd();

    //wraps around indexes
    width_index++;
    if(width_index == width_item_count) {
        width_index = 0;
        height_index++;
        if(height_index == height_item_count) {
            height_index = 0;
        }
    }

}
