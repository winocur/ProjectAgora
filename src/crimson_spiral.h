#ifndef crimson_spiral
#define crimson_spiral

struct GameOffscreenBuffer {
    void * memory;
    int width;
    int height;
    int pitch;
};

struct GameOutputSoundBuffer {
    int samples_per_second;
    int sample_count;
    i16 * samples;
    //int running_sample_index;
};

void game_update_and_render (GameOffscreenBuffer * game_buffer, GameOutputSoundBuffer * sound_buffer) ;

void render_weird_gradient (GameOffscreenBuffer * buffer, int x_offset, int y_offset) ;

void game_output_sound (GameOutputSoundBuffer * sound_buffer, int tone_hz);


//input
struct GameButtonState {
    int half_transition_count;
    bool ended_down;
};

struct GameControllerInput {
    bool is_analog;

    f32 start_x, start_y;
    f32 min_x, min_y;
    f32 max_x, max_y;
    f32 end_x, end_y;

    union {
        GameButtonState buttons [6];
        struct {
            GameButtonState up;
            GameButtonState down;
            GameButtonState left;
            GameButtonState right;
            GameButtonState left_shoulder;
            GameButtonState right_shoulder;
        };
    };
};

struct GameInput {
    GameControllerInput controllers [4];
};


#endif
