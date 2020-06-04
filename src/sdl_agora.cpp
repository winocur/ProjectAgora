#define MAX_CONTROLLERS 4

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL_opengl.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include "sdl_agora.h"

SdlWindowDimension windowDimension;

#include "agora.h"
#include "agora.cpp"


int main(int argc, char *argv[]) {

	//Initialization
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
		printf("Error initializing SDL");
		printf("ERR: %s", SDL_GetError());
	}
	
	// Initialize truetype
	if (TTF_Init() < 0) {
    	printf("TTF failed to initialize");
		return 0;
	}

	SDL_RendererInfo rendererInfo;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Surface * windowSurface;
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer);

	char* windowTitle = new char[64];

	if(window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	//Initialize IMG flags for PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }

	windowSurface = SDL_GetWindowSurface(window);
	windowDimension = SdlGetWindowDimension(window);

	SDL_GetRendererInfo(renderer, &rendererInfo);
    if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
        (rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
        OutputDebug("this is borked!");
    }

	// Initialize openGL through SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
	SDL_GL_CreateContext(window);

	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
  		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	InitGL (windowDimension.width, windowDimension.height);

	//TODO catch errors
	GameMemory memory;
	memory.permanentStorageSize = MEGABYTES(64);
	memory.permanentStorage = malloc(memory.permanentStorageSize);
	memory.temporaryStorageSize = GIGABYTES(1);
	memory.temporaryStorage = malloc(memory.temporaryStorageSize);
	memory.temporaryStorageCurrent = memory.temporaryStorage;

	GameInit(windowSurface, &memory);

	// -- MAIN GAME LOOP -- //
	bool running = true;

	// minimum fps a frame can take
	const f64 fpsCap = 144.f;
	const f64 msMinCap = (1000.f / fpsCap);

	f64 msElapsed = 0.f;
	f64 FPS = 0.f;

	u64 performanceFrequency = SDL_GetPerformanceFrequency();
	u64 lastCounter = SDL_GetPerformanceCounter();

	
	while(running) {

		GameInputFrame input;
		input.cameraX = 0;
		input.cameraY = 0;
		input.leftButton = 0;
		input.middleButton = 0;
		input.rightButton = 0;

		sprintf(windowTitle, "Agora fps: %f", FPS);
		SDL_SetWindowTitle(window, windowTitle);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
				if (HandleEvent(&event, &input)) {
				running = false;
			}
		}
		
		windowDimension = SdlGetWindowDimension(window);

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		input.mousePosition = { ((float)mouseX), windowDimension.height - ((float)mouseY) };

	  	GameUpdateAndRender(&memory, windowDimension.width, windowDimension.height, msElapsed, input);

		SDL_GL_SwapWindow(window);

		u64 endCounter = SDL_GetPerformanceCounter();
		u64 counterElapsed = endCounter - lastCounter;
		msElapsed = ((1000.0f * (f64)counterElapsed) / (f64) performanceFrequency);
		FPS = ((f64) performanceFrequency / (f64)counterElapsed);

		// if the frame took too short we delay some ms and recalculate
		if(msElapsed < msMinCap) {
			SDL_Delay(msMinCap - msElapsed);

			endCounter = SDL_GetPerformanceCounter();
			counterElapsed = endCounter - lastCounter;

			msElapsed = ((1000.0f * (f64)counterElapsed) / (f64) performanceFrequency);
			FPS = fpsCap;
		}

		lastCounter = endCounter;
	} // -- END GAME LOOP -- //

	delete [] windowTitle;

	SDL_DestroyWindow (window);

	Cleanup(&memory);

	return 0;
}

void Cleanup (GameMemory * gm) {
		OutputDebug("Cleaning up resources");
		GameCleanup(gm);
		SDL_CloseAudio();
		SDL_Quit();
		TTF_Quit();
}

void OutputDebug(char * message) {
	printf("%s", message);
	printf("\n");
}

bool HandleEvent (SDL_Event * event, GameInputFrame * input) {
	switch (event->type) {
		case SDL_MOUSEBUTTONDOWN : {
			switch (event->button.button) {
				case SDL_BUTTON_LEFT : {
					input->leftButton = 1;
					break;
				}
				case SDL_BUTTON_MIDDLE : {
					input->middleButton = 1;
					break;
				}
				case SDL_BUTTON_RIGHT : {
					input->rightButton = 1;
					break;
				}

			}
			break;
		}
		case SDL_MOUSEBUTTONUP : {
			switch (event->button.button) {
				case SDL_BUTTON_LEFT : {
					input->leftButton = 2;
					break;
				}
				case SDL_BUTTON_MIDDLE : {
					input->middleButton = 2;
					break;
				}
				case SDL_BUTTON_RIGHT : {
					input->rightButton = 2;
					break;
				}

			}
		} break;
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
				wasDown = false;
			}
			if(keycode == SDLK_a) {
				if(wasDown) {
					input->cameraX = 0;
				} else {
					input->cameraX = -1.f;
				}
			} else if(keycode == SDLK_s) {
				if(wasDown) {
					input->cameraY = 0;
				} else {
					input->cameraY = -1.f;
				}
			} else if(keycode == SDLK_d) {
				if(wasDown) {
					input->cameraX = 0;
				} else {
					input->cameraX = 1.f;
				}
			} else if(keycode == SDLK_w) {
				if(wasDown) {
					input->cameraY = 0;
				} else {
					input->cameraY = 1.f;
				}
			} else if(keycode == SDLK_z) {

			} else if(keycode == SDLK_x) {

			//arrow keys
			} else if (keycode == SDLK_DOWN) {
				if(wasDown) {
					input->cameraY = 0;
				} else {
					input->cameraY = 1;
				}
			} else if (keycode == SDLK_UP) {
				if(wasDown) {
					input->cameraY = 0;
				} else {
					input->cameraY = -1;
				}
			} else if (keycode == SDLK_LEFT) {
				if(wasDown) {
					input->cameraX = 0;
				} else {
					input->cameraX = 1;
				}
			} else if (keycode == SDLK_RIGHT) {
				if(wasDown) {
					input->cameraX = 0;
				} else {
					input->cameraX = -1;
				}
			} else if (keycode == SDLK_o) {
				if(wasDown) {
					input->zoomChange = 0;
				} else {
					input->zoomChange = 0.05;
				}
			} else if (keycode == SDLK_p) {
				if(wasDown) {
					input->zoomChange = 0;
				} else {
					input->zoomChange = -0.05;
				}

			} else if(keycode == SDLK_RETURN) {
				
			} else if(keycode == SDLK_ESCAPE) {
				//single keystroke down
				if(!wasDown) {
					OutputDebug("ESCAPE event!");
					return true;
				}
			}
		} break;
	}
	return false;
}


bool InitGL (int width, int height) {

	f32 ratio = ((f32) width) / ((f32) height);

	/* Our shading model--Gouraud (smooth). */
    glShadeModel(GL_SMOOTH);

	//Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 0.f);

	bool success = true;
    GLenum error = GL_NO_ERROR;

	glViewport( 0.f, 0.f, width, height);

	//Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	//glOrtho( 0, width, 0, height, 1.0, -1.0 );

	//gluPerspective(60.f,ratio,1,1000);

	//Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	//pitagoras only works with 60 degree angle
	f32 cameraDistance = sqrt((height * height) - (height/2) * (height/2));

	// try D =(h/2)/(tan a) for custom angle

	/*
	gluLookAt(
		// eye position
		width/2,height/2, cameraDistance,
		//center of projection
		width/2,height/2,0,
		//up vector
		0, 1, 0);
	*/

	glPushMatrix();

	glShadeModel(GL_SMOOTH);             //Enables Smooth Color Shading
	
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
		printf("InitGL -> Initialized succesfully\n");
	}

    return success;
}

SdlWindowDimension SdlGetWindowDimension (SDL_Window *window) {
	SdlWindowDimension result;
    SDL_GetWindowSize(window, &result.width, &result.height);
    return result;
}
