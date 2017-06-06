#define MAX_CONTROLLERS 4
#define internal static

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdl_crimson_spiral.h"

#include <SDL_opengl.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "crimson_spiral.h"
#include "crimson_spiral.cpp"

static SdlWindowDimension windowDimension;

int main(int argc, char *argv[]) {

	//Initialization
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
		printf("Error initializing SDL");
		printf("ERR: %s", SDL_GetError());
	}

	SDL_RendererInfo rendererInfo;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer);

	SDL_SetWindowTitle(window, "Crimson Spiral (Prototype)");


	if( window == NULL )
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
	}

	windowDimension = SdlGetWindowDimension(window);


	SDL_GetRendererInfo(renderer, &rendererInfo);
    if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        OutputDebug("thus is borked!");
    }


	SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
  	printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	InitGL (windowDimension.width, windowDimension.height);

	GameInit();

	// -- MAIN GAME LOOP -- //
	bool running = true;

	while(running) {

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
				if (HandleEvent(&event)) {
				running = false;
			}
		}

		const int msDelay = 16;

		windowDimension = SdlGetWindowDimension(window);

	  	GameUpdateAndRender(windowDimension.width, windowDimension.height, msDelay);

		SDL_GL_SwapWindow(window);
		//SDL_GL_SwapBuffers();
		SDL_Delay( msDelay );
	} // -- END GAME LOOP -- //

	SDL_DestroyWindow( window );
	Cleanup();

	return 0;
}

void Cleanup () {
		OutputDebug("Cleaning up resources");
		SDL_CloseAudio();
		SDL_Quit();
}

void OutputDebug(char * message) {
	printf("%s", message);
	printf("\n");
}

bool HandleEvent (SDL_Event * event) {
	switch (event->type) {
		case SDL_QUIT: {
			OutputDebug("SDL_QUIT");
			return true;
		} break;

		case SDL_WINDOWEVENT: {
			switch(event->window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED: {
					windowDimension.width = event->window.data1;
					windowDimension.height = event->window.data2;
				}break;
				case SDL_WINDOWEVENT_EXPOSED:
				{

				}break;
			}
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP: {
			bool wasDown = false;
			SDL_Keycode keycode = event->key.keysym.sym;
			if(event->key.state == SDL_RELEASED) {
				wasDown = true;
			} else if (event->key.repeat != 0) {
				wasDown = true;
			}
			if(keycode == SDLK_a) {

			} else if(keycode == SDLK_s) {

			} else if(keycode == SDLK_d) {

			} else if(keycode == SDLK_w) {

			} else if(keycode == SDLK_z) {

			} else if(keycode == SDLK_x) {

			} else if(keycode == SDLK_RETURN) {

			} else if(keycode == SDLK_ESCAPE) {
				//single keystroke down
				if(!wasDown) {
					OutputDebug("ESCAPE event!");
				}
			}
		} break;
	}
	return false;
}


bool InitGL (int width, int height) {

	float ratio = (float) width / (float) height;

	/* Our shading model--Gouraud (smooth). */
    glShadeModel(GL_SMOOTH);

	//Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 0.f);

	bool success = true;
    GLenum error = GL_NO_ERROR;


	//Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	//Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	glShadeModel(GL_SMOOTH);             //Enables Smooth Color Shading
	glEnable(GL_TEXTURE_2D);

	//blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "InitGL -> Error initializing OpenGL! %s\n", gluErrorString( error ) );
        success = false;
    } else {
		printf("InitGL -> Initialized succesfully");
	}

    return success;
}

SdlWindowDimension SdlGetWindowDimension (SDL_Window *window) {
	SdlWindowDimension result;
    SDL_GetWindowSize(window, &result.width, &result.height);
    return result;
}
