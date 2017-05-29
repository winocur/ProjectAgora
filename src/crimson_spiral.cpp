#include "crimson_spiral.h"

#include <GL\GLU.h>
#include <GL\GL.h>
#include <SDL_opengl.h>

#include "sprites.cpp"

void game_init() {

    load_sprite("../assets/test_spritesheet.bmp");
}

void game_update_and_render (int screen_width, int screen_height) {

    GLenum error = GL_NO_ERROR;

    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    render_sprite(screen_width, screen_height);

    error = glGetError();
    if( error != GL_NO_ERROR )
    {
      printf( "Error initializing OpenGL!");
    }

    //game_output_sound(sound_buffer, tone_hz);
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
