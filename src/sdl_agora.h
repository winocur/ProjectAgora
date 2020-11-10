#ifndef H_SDL_AGORA
#define H_SDL_AGORA

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

#define KILOBYTES(value) ((value) * 1024)
#define MEGABYTES(value) (KILOBYTES(value) * 1024)
#define GIGABYTES(value) (MEGABYTES(value) * 1024)

#include <stdint.h>
#include <math.h>
#include <SDL.h>

#include "agora.h"

bool HandleEvent(SDL_Event* event, GameInputFrame* input);

WindowDimension SdlGetWindowDimension (SDL_Window * window);

void Cleanup (GameMemory * gm);

void OutputDebug(char * message);

//Initializes matrices and clear color
bool InitGL (int width, int height);

#endif  /* H_SDL_CRIMSONSPIRAL */
