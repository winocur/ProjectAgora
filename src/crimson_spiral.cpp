#include "crimson_spiral.h"

#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL_opengl.h>

#include "sprites.cpp"

//TODO find some nice place to store this!
SpriteSheet * running_dude;
SpriteSheet * other_dude;
SpriteAnimation * running_dude_animation_1;
SpriteAnimation * running_dude_animation_2;
SpriteAnimation * other_dude_animation;


void game_init() {

    running_dude = load_sprite_sheet("../assets/test_spritesheet.bmp", GL_BGR, 6, 5);
    other_dude = load_sprite_sheet("../assets/other_dude.bmp", GL_BGRA, 9, 4);
    other_dude_animation = load_sprite_animation (other_dude, "other_dude", 64, true, 0, 9, 2, 3);
    running_dude_animation_1 = load_sprite_animation(running_dude, "running_dude1", 16, true, 0, 6, 0, 5);
    running_dude_animation_2 = load_sprite_animation(running_dude, "running_dude2", 32, true, 0, 6, 0, 5);
}

void game_update_and_render (int screen_width, int screen_height, int ms_elapsed) {

    GLenum error = GL_NO_ERROR;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    render_sprite_animation(running_dude_animation_1, 0.5f, 0.5f, screen_width, screen_height, ms_elapsed);
    render_sprite_animation(running_dude_animation_2, -0.5f, -0.5f, screen_width, screen_height, ms_elapsed, 1.5f, true);
    render_sprite_animation(other_dude_animation, -0.5f, 0.5f, screen_width, screen_height, ms_elapsed, 3.f, false);

    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf("OpenGL Error: %s\n", gluErrorString( error ));
    }

    //game_output_sound(sound_buffer, tone_hz);
}

void game_cleanup() {
    unload_sprite_animation(running_dude_animation_1);
    unload_sprite_animation(running_dude_animation_2);
    unload_sprite_animation(other_dude_animation);
    //TODO unload sprite sheets
}

void game_output_sound (GameOutputSoundBuffer * sound_buffer, int tone_hz) {

    local_persist f32 tSine;
    int wave_period = sound_buffer->samples_per_second / tone_hz;
	int tone_volume = 5000;

    i16 * sample_out = (i16 *) sound_buffer->samples;

	for(int i = 0; i <  sound_buffer->sample_count; i++) {

        f32 sine_value = sinf(tSine);
        i16 sample_value = (i16)(sine_value *  tone_volume);

		*sample_out++ = sample_value;
		*sample_out++ = sample_value;

        tSine += 2.0f * PI32 * 1.0f / (f32) wave_period;

	}
}
