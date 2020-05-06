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

struct GameInputFrame {
    f32 cameraX;
    f32 cameraY;

    f32 zoomChange;
    
    Vector2 mousePosition;
    
    u8 leftButton;    
    u8 rightButton;
    u8 middleButton;
};

struct SdlWindowDimension {
    int width;
    int height;
};

bool HandleEvent(SDL_Event* event, GameInputFrame* input);

SdlWindowDimension SdlGetWindowDimension (SDL_Window * window);

void Cleanup (GameMemory * gm);

void OutputDebug(char * message);

//Initializes matrices and clear color
bool InitGL (int width, int height);

#endif  /* H_SDL_CRIMSONSPIRAL */
