#ifndef sdl_crimson_spiral
#define sdl_crimson_spiral

//rust-like numerical types
typedef uint8_t 	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;
typedef uint64_t 	u64;
typedef int8_t 		i8;
typedef int16_t 	i16;
typedef int32_t 	i32;
typedef int64_t 	i64;
typedef float 		f32;
typedef double		f64;


#define PI32 3.14159265359
#define local_persist   static
#define internal        static
#define global_variable static

#include <stdint.h>
#include <math.h>
#include <SDL.h>

#include "crimson_spiral.h"


struct SdlOffscreenBuffer {
    SDL_Texture *texture;
    void 		*memory;
    int			memory_size;
    int 		width;
    int 		height;
    int         pitch;
    int 	    bytes_per_pixel;
};

struct SdlSoundBuffer {
    int     tone_hz;
    i16     tone_volume;
    u32     running_sample_index;
    int     wave_period;
    int     bytes_per_sample;
    int     samples_per_second;
    int     latency_sample_count;
    void *  buffer;
};

struct SdlWindowDimension {
    int width;
    int height;
};

bool handle_event(SDL_Event* event, SdlOffscreenBuffer *buffer);

void sdl_resize_texture (SDL_Renderer *renderer, SdlOffscreenBuffer *buffer, int width, int height);

void sdl_update_window (SDL_Renderer *renderer, SdlOffscreenBuffer *buffer);

SdlWindowDimension sdl_get_window_dimension (SDL_Window * window);

void cleanup ();

void output_debug(char * message);

void sdl_audio_callback (void * user_data, u8 * audio_data, int length);

void sdl_fill_sound_buffer (SdlSoundBuffer * sound_buffer, GameOutputSoundBuffer * game_sound_buffer, int bytes_to_write);


#endif
