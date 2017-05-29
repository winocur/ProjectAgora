#define MAX_CONTROLLERS 4
#define internal static

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdl_crimson_spiral.h"
#include <sys/mman.h>
#include <x86intrin.h>

#include "crimson_spiral.h"
#include "crimson_spiral.cpp"

bool running = true;

static SdlWindowDimension window_dimension;
static SdlOffscreenBuffer global_backbuffer;
static SDL_GameController * controller_handles[MAX_CONTROLLERS];
static bool audio_is_playing = false;
static int y_offset = 0;

void sdl_init_audio (int samples_per_second, int buffer_size) {
	SDL_AudioSpec audio_settings = {0};
	audio_settings.freq = samples_per_second;
	audio_settings.channels = 2;
	audio_settings.format = AUDIO_S16LSB;
	audio_settings.samples = buffer_size;
	SDL_OpenAudio(&audio_settings, 0);
	if(audio_settings.format != AUDIO_S16) {
		output_debug("ERROR: cant get audio format");
		SDL_CloseAudio();
	}
}

int main(int argc, char *argv[]) {

	//Initialization
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
		printf("Error initializing SDL");
		printf("ERR: %s", SDL_GetError());
	}

	SDL_Window *window = SDL_CreateWindow("Crimson Spiral", 0, 0, 500, 500, SDL_WINDOW_RESIZABLE);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	window_dimension = sdl_get_window_dimension(window);

	//Load game controllers
	int joystick_count = SDL_NumJoysticks();
	int controller_index;
	for(int i = 0; i < joystick_count; i++) {
		if(!SDL_IsGameController(i)) { continue; }
		if(controller_index > MAX_CONTROLLERS) { break; }
		printf("I found a controller!");
		controller_handles[controller_index] = SDL_GameControllerOpen(i);
		controller_index++;
	}

	//texture
	global_backbuffer;
	global_backbuffer.width = window_dimension.width;
	global_backbuffer.height = window_dimension.height;
	global_backbuffer.bytes_per_pixel = 4;
	global_backbuffer.memory_size = global_backbuffer.width * global_backbuffer.height
	 									* global_backbuffer.bytes_per_pixel;
	global_backbuffer.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
												SDL_TEXTUREACCESS_STREAMING,
												 global_backbuffer.width, global_backbuffer.height);
	global_backbuffer.memory = mmap(0, global_backbuffer.memory_size, PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	sdl_resize_texture(renderer, &global_backbuffer, window_dimension.width, window_dimension.height);

	//loop
	int x_offset = 0;

	//Sound buffer
	SdlSoundBuffer sound_output = {};
	sound_output.bytes_per_sample = sizeof(i16) * 2;
	sound_output.samples_per_second = 48000;
    sound_output.latency_sample_count = sound_output.samples_per_second / 2;
	sdl_init_audio(sound_output.samples_per_second, sound_output.samples_per_second / sound_output.bytes_per_sample * 30);
	i16 * samples = (i16 *)calloc(sound_output.latency_sample_count, sound_output.bytes_per_sample);
    SDL_PauseAudio(0);

	//performance counter
	u64 last_counter = SDL_GetPerformanceCounter();
	u64 counter_frecuency = SDL_GetPerformanceFrequency();
	u64 last_cycles = _rdtsc();

	// -- MAIN GAME LOOP -- //
	while(running) {

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
    		if (handle_event(&event, &global_backbuffer)) {
				running = false;
			}
    }

		GameOutputSoundBuffer game_sound_buffer = {};
		int target_queue_bytes = sound_output.latency_sample_count * sound_output.bytes_per_sample;
        int bytes_to_write = target_queue_bytes - SDL_GetQueuedAudioSize(1);
		game_sound_buffer.samples_per_second = sound_output.samples_per_second;
		game_sound_buffer.sample_count =  bytes_to_write / sound_output.bytes_per_sample;
		game_sound_buffer.samples = samples;

		GameOffscreenBuffer game_buffer = {};
		game_buffer.memory = global_backbuffer.memory;
		game_buffer.width  = global_backbuffer.width;
		game_buffer.height = global_backbuffer.height;
		game_buffer.pitch  = global_backbuffer.pitch;

		GameInput input [2] = {};
		GameInput *old_input = &input[0];
		GameInput *new_input = &input[1];

		game_update_and_render (&game_buffer, &game_sound_buffer) ;

		sdl_fill_sound_buffer(&sound_output, &game_sound_buffer, bytes_to_write);

		x_offset++;
		sdl_update_window(renderer, &global_backbuffer);

		//controller input
		for(int i = 0; i < MAX_CONTROLLERS; i++) {
			if(controller_handles[i] != 0 && SDL_GameControllerGetAttached(controller_handles[i])) {

				GameControllerInput *old_controller = &old_input->controllers[i];
				GameControllerInput *new_controller = &new_input->controllers[i];

				bool LeftShoulder = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);


				bool Up = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_DPAD_UP);
		        bool Down = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		        bool Left = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		        bool Right = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
		        bool Start = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_START);
		        bool Back = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_BACK);
		        bool RightShoulder = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
		        bool AButton = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_A);
		        bool BButton = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_B);
		        bool XButton = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_X);
				bool YButton = SDL_GameControllerGetButton(controller_handles[i], SDL_CONTROLLER_BUTTON_Y);

		        i16 StickX = SDL_GameControllerGetAxis(controller_handles[i], SDL_CONTROLLER_AXIS_LEFTX);
		        i16 StickY = SDL_GameControllerGetAxis(controller_handles[i], SDL_CONTROLLER_AXIS_LEFTY);

				//TEST
				if(AButton) {
					y_offset++;
				}
			}
		}

		//performance counting
		u64 end_counter = SDL_GetPerformanceCounter();
		u64 counter_elapsed = end_counter - last_counter;
		f64 ms_per_frame = ((1000.0f * (f64)counter_elapsed)) / (f64)counter_frecuency;
		f64 fps =  (f64)counter_frecuency / (f64)counter_elapsed;
		u64 end_cycles = _rdtsc();
		u64 cycles_elapsed = end_cycles - last_cycles;
		//megacycles per frame
		f64 mcpf = (f64)cycles_elapsed / (1000.0f * 1000.0f);

		//printf("%fms/f, %ff/s, %fmc/f\n", ms_per_frame, fps, mcpf);
		last_counter = end_counter;
		last_cycles = end_cycles;
	} // -- END GAME LOOP -- //

	//Cleanup
	cleanup();
	return 0;
}

void sdl_process_game_controller_button (GameButtonState * old_state, GameButtonState * new_state,
	 			SDL_GameController * controller_handle, SDL_GameControllerButton button) {

	new_state->ended_down = SDL_GameControllerGetButton(controller_handle, button);
	new_state->half_transition_count += ((new_state->ended_down == old_state->ended_down) ? 0 : 1);
}

void sdl_fill_sound_buffer (SdlSoundBuffer * sound_buffer, GameOutputSoundBuffer * game_sound_buffer, int bytes_to_write) {
	SDL_QueueAudio(1,  game_sound_buffer->samples, bytes_to_write);
}

SdlWindowDimension sdl_get_window_dimension (SDL_Window *window) {
	SdlWindowDimension result;
    SDL_GetWindowSize(window, &result.width, &result.height);
    return result;
}

void cleanup () {

	output_debug("Cleaning up resources");
	SDL_CloseAudio();
	for(int i = 0; i < MAX_CONTROLLERS; i++) {
		if(controller_handles[i]) {
			SDL_GameControllerClose(controller_handles[i]);
		}
	}
	SDL_Quit();
}

bool handle_event(SDL_Event * event, SdlOffscreenBuffer *buffer) {
	switch (event->type) {
		case SDL_QUIT: {
			output_debug("SDL_QUIT");
			return true;
		} break;

		case SDL_WINDOWEVENT: {
			switch(event->window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED: {
					SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
					SDL_Renderer *renderer = SDL_GetRenderer(window);
					window_dimension.width = event->window.data1;
					window_dimension.height = event->window.data2;
					sdl_update_window(renderer, buffer);
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
				//holding the key down
				y_offset+=3;
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

void sdl_update_window (SDL_Renderer *renderer, SdlOffscreenBuffer *buffer) {
	if(SDL_UpdateTexture(buffer->texture, 0, buffer->memory, buffer->pitch)) {
		//TODO: ERROR check
	}
	SDL_RenderCopy(renderer, buffer->texture, 0, 0);
	SDL_RenderPresent(renderer);
}


void sdl_resize_texture (SDL_Renderer *renderer, SdlOffscreenBuffer *buffer, int width, int height) {

	//cleanup
	if(buffer->texture) { SDL_DestroyTexture(buffer->texture); }
	if(buffer->memory) { munmap(buffer->memory, buffer->memory_size); }

	buffer->width = width;
	buffer->memory_size = buffer->width * buffer->height * buffer->bytes_per_pixel;
	//amount of bytes for each line of pixels: pitch
	buffer->pitch = buffer->width * buffer->bytes_per_pixel;

	//texture
	buffer->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
											SDL_TEXTUREACCESS_STREAMING, width, height);

	buffer->memory = mmap(0, buffer->memory_size , PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

}

void sdl_audio_callback (void * user_data, u8 * audio_data, int length) {
	memset(audio_data, 0, length);
}

void output_debug(char * message) {
	printf("%s", message);
	printf("\n");
}
