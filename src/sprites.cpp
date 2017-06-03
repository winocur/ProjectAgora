

#include "sprites.h"

#include <GL/gl.h>
#include <SDL_opengl.h>


SpriteSheet * load_sprite_sheet(char* filepath, GLenum type, int x_count, int y_count) {
    SpriteSheet * sheet = new SpriteSheet;

    /* Create storage space for the texture */
    SDL_Surface * texture_image[1];

    if (texture_image[0] = SDL_LoadBMP(filepath)) {

        glGenTextures( 1, &sheet->texture );

        glBindTexture(GL_TEXTURE_2D, sheet->texture);

        sheet->width  = texture_image[0]->w;
        sheet->height = texture_image[0]->h;
        sheet->x_count = x_count;
        sheet->y_count = y_count;

        sheet->frame_width  = sheet->width / sheet->x_count;
        sheet->frame_height = sheet->height / sheet->y_count;

        glTexImage2D(GL_TEXTURE_2D, 0, 3, sheet->width,
            //need alpha channel support or something yeahhh
              sheet->height, 0, type,
              GL_UNSIGNED_BYTE, texture_image[0]->pixels);

        //TODO error handling
        glGetError();

        /* Linear Filtering */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    } else {
        printf("failed to load spritesheet");
    }

    if (texture_image[0]) {
        SDL_FreeSurface(texture_image[0]);
    }

    return sheet;
}

SpriteAnimation * load_sprite_animation (SpriteSheet * sprite_sheet,
                                        char* identifier,
                                        int frame_duration,
                                        bool is_loop,
                                        int x_start, int x_end,
                                        int y_start, int y_end) {

    SpriteAnimation * animation = new SpriteAnimation;

    animation->identifier = identifier;
    animation->is_loop    = is_loop;
    animation->frame_duration = frame_duration;
    animation->frame_duration_counter = 0;
    animation->current_frame = 0;
    animation->sprite_sheet = sprite_sheet;

    animation->frame_count = 0;
    for(int y = y_start; y < y_end; ++y) {
        for(int x = x_start; x < x_end; ++x) {
            animation->frame_count++;
        }
    }

    if(animation->frame_count > MAX_FRAMES) {
        printf("You are trying to allocate a sprite animation with more than the maximum frames allowed");
    }

    int frame = 0;

    //TODO end is not inclusive and beginning is inclusive
    //super ugly, make better
    for(int y = y_start; y < y_end; ++y) {
        for(int x = x_start; x < x_end; ++x) {

            animation->frames[frame].x_index = x;
            animation->frames[frame].y_index = y;
            ++frame;
        }
    }

    return animation;
}

void unload_sprite_animation (SpriteAnimation * sprite_animation) {
    delete [] sprite_animation->frames;
    delete sprite_animation;
}


void render_sprite_animation (SpriteAnimation * sprite_animation,
                                float x, float y,
                                int screen_width, int screen_height,
                                int ms_elapsed, float scale = 1.f,
                                bool is_flipped = false) {

    //moves the counter and moves the current frame if needed
    sprite_animation->frame_duration_counter += ms_elapsed;

    while(sprite_animation->frame_duration_counter >= sprite_animation->frame_duration) {

        sprite_animation->current_frame++;

        //reached the end of the animation
        if(sprite_animation->current_frame == sprite_animation->frame_count) {
            if(sprite_animation->is_loop) {
                sprite_animation->current_frame = 0;
            } else {
                sprite_animation->current_frame--;
            }
        }

        // moves the counter back
        sprite_animation->frame_duration_counter -= sprite_animation->frame_duration;
    }

    const SpriteSheet * sprite_sheet = sprite_animation->sprite_sheet;
    const SpriteSheetFrame current_frame = sprite_animation->frames[sprite_animation->current_frame];

    float relative_width_unit = 1.f / sprite_sheet->x_count;
    float relative_height_unit = 1.f / sprite_sheet->y_count;

    float sprite_width = scale * sprite_sheet->width / sprite_sheet->x_count / (float)screen_width;
    float sprite_height = scale * sprite_sheet->height / sprite_sheet->y_count / (float)screen_height;

    //centered around insertion point
    float x_position = x - (sprite_width / 2);
    float y_position = y - (sprite_height / 2);

    //bind texture
    glBindTexture(GL_TEXTURE_2D, sprite_sheet->texture);

    glBegin(GL_QUADS);
        //tint
        glColor4d(1, 1, 1, 1);

        //OpenGL
        //top left
        glTexCoord2f(relative_width_unit * current_frame.x_index,          //x
                    relative_height_unit * (current_frame.y_index + 1));   //y
        if(is_flipped) {
            glVertex2f( x_position + sprite_width, y_position);
        } else {
            glVertex2f( x_position, y_position);
        }
        //top right
        glTexCoord2f(relative_width_unit * (current_frame.x_index + 1),    //x
                    relative_height_unit * (current_frame.y_index + 1));   //y
        if(is_flipped) {
            glVertex2f( x_position, y_position);
        } else {
            glVertex2f( x_position + sprite_width, y_position);
        }

        //bot right
        glTexCoord2f(relative_width_unit * (current_frame.x_index + 1),    //x
                    relative_height_unit * current_frame.y_index);         //y
        if(is_flipped) {
            glVertex2f( x_position, y_position + sprite_height);
        } else {
            glVertex2f( x_position + sprite_width, y_position + sprite_height);
        }
        //bot left
        glTexCoord2f(relative_width_unit * current_frame.x_index,          //x
                    relative_height_unit * current_frame.y_index);         //y
        if(is_flipped) {
            glVertex2f( x_position + sprite_width, y_position + sprite_height);
        } else {
            glVertex2f( x_position, y_position + sprite_height);
        }
    glEnd();
}

void reset_sprite_animation (SpriteAnimation * sprite_animation) {
    sprite_animation->current_frame = 0;
    sprite_animation->frame_duration_counter = 0;
}



// -- test code -- //

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
              texture_height , 0, GL_BGR,
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
