#ifndef sprites
#define sprites

#include <GL/glu.h>

#define MAX_FRAMES 256

struct SpriteSheet {
    int width;
    int height;
    int x_count;
    int y_count;
    int frame_width;
    int frame_height;
    GLuint texture;
};

struct SpriteSheetFrame {
    int x_index;
    int y_index;
};

struct SpriteAnimation {
    SpriteSheetFrame frames [MAX_FRAMES];
    int frame_count;

    char* identifier;
    SpriteSheet* sprite_sheet;

    bool is_loop;
    int current_frame;
    int frame_duration;
    int frame_duration_counter;

    float relative_width_unit;
    float relative_height_unit;

};

SpriteSheet * load_sprite_sheet(char* filepath, GLenum type, int x_count, int y_count) ;

SpriteAnimation * load_sprite_animation (SpriteSheet * sprite_sheet,
                                        char* identifier,
                                        int frame_duration,
                                        bool is_loop,
                                        int x_start, int x_end,
                                        int y_start, int y_end
                                        );

void unload_sprite_animation (SpriteAnimation * sprite_animation);

void render_sprite_animation (SpriteAnimation * sprite_animation,
                                float x, float y,
                                int screen_width, int screen_height,
                                int ms_elapsed, float scale,
                                bool is_flipped);

void reset_sprite_animation (SpriteAnimation * sprite_animation);


void load_sprite(char* path) ;

void render_sprite(int screen_width, int screen_height) ;



#endif
