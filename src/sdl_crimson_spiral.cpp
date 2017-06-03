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

static SdlWindowDimension window_dimension;

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

	window_dimension = sdl_get_window_dimension(window);


	SDL_GetRendererInfo(renderer, &rendererInfo);
    if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        output_debug("thus is borked!");
    }


	SDL_GL_CreateContext(window);

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
  	printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	init_gl(window_dimension.width, window_dimension.height);

	game_init();

	// -- MAIN GAME LOOP -- //
	bool running = true;

	while(running) {

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
				if (handle_event(&event)) {
				running = false;
			}
		}

		const int ms_delay = 16;

		window_dimension = sdl_get_window_dimension(window);

	  	game_update_and_render(window_dimension.width, window_dimension.height, ms_delay);

		SDL_GL_SwapWindow(window);
		//SDL_GL_SwapBuffers();
		SDL_Delay( ms_delay );
	} // -- END GAME LOOP -- //

	SDL_DestroyWindow( window );
	cleanup();

	return 0;
}

void cleanup () {
		output_debug("Cleaning up resources");
		SDL_CloseAudio();
		SDL_Quit();
}

void output_debug(char * message) {
	printf("%s", message);
	printf("\n");
}

bool handle_event(SDL_Event * event) {
	switch (event->type) {
		case SDL_QUIT: {
			output_debug("SDL_QUIT");
			return true;
		} break;

		case SDL_WINDOWEVENT: {
			switch(event->window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED: {
					window_dimension.width = event->window.data1;
					window_dimension.height = event->window.data2;
				}break;
				case SDL_WINDOWEVENT_EXPOSED:
				{

				}break;
			}
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP: {
			bool was_down = false;
			SDL_Keycode key_code = event->key.keysym.sym;
			if(event->key.state == SDL_RELEASED) {
				was_down = true;
			} else if (event->key.repeat != 0) {
				was_down = true;
			}
			if(key_code == SDLK_a) {

			} else if(key_code == SDLK_s) {

			} else if(key_code == SDLK_d) {

			} else if(key_code == SDLK_w) {

			} else if(key_code == SDLK_z) {

			} else if(key_code == SDLK_x) {

			} else if(key_code == SDLK_RETURN) {

			} else if(key_code == SDLK_ESCAPE) {
				//single keystroke down
				if(!was_down) {
					output_debug("ESCAPE event!");
				}
			}
		} break;
	}
	return false;
}


bool init_gl (int width, int height) {

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
        printf( "init_gl -> Error initializing OpenGL! %s\n", gluErrorString( error ) );
        success = false;
    } else {
		printf("init_gl -> Initialized succesfully");
	}

    return success;
}

SdlWindowDimension sdl_get_window_dimension (SDL_Window *window) {
	SdlWindowDimension result;
    SDL_GetWindowSize(window, &result.width, &result.height);
    return result;
}
