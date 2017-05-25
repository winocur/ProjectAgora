#include "nova.h"

void game_update_and_render (GameOffscreenBuffer * game_buffer, GameOutputSoundBuffer * sound_buffer) {

    local_persist int blue_offset = 0;
    local_persist int green_offset = 0;
    local_persist int tone_hz = 256;

    render_weird_gradient (game_buffer, blue_offset, green_offset);

    blue_offset++;
    green_offset++;

    game_output_sound(sound_buffer, tone_hz);
}

void render_weird_gradient (GameOffscreenBuffer * buffer, int blue_offset, int green_offset) {

	int bytes_per_pixel = 4;

	u8 * row = (u8 *) buffer->memory;
	for (int y = 0; y < buffer->height; y++) {

		u32 * pixel = (u32 *) row;
		for(int x = 0; x < buffer->width; x++) {

			u8 blue = (x + blue_offset);
			u8 green = (y + green_offset);

			*pixel++ = ((green << 8) | blue);
		}

		row += buffer->pitch;
	}
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
