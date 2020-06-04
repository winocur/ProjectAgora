#ifndef H_INPUT
#define H_INPUT

enum MouseButtonEvent {
    NONE,
    CLIC,
    LEFT_CLIC,
    DRAG_ITEM,
    DRAG_CAMERA,
    ZOOMIN,
    ZOOMOUT,
};

enum MouseButtonState {
    UP,
    DOWN
};


struct MouseEvents {
    MouseButtonEvent event;

    Vector2 mouseDelta;
    Vector2 mousePosition;

    f32 zoomChange;
};

struct GameInputState {

    Vector2 mousePosition;

    MouseButtonState leftButton;    
    MouseButtonState rightButton;
    MouseButtonState middleButton;
};

MouseEvents
ProcessMouseInput (const GameInputFrame* inputFrame) ;



#endif // H_INPUT