
#include "input.h"

const f32 displacementThreshold = 2.0;

MouseEvents ProcessMouseInput (const GameInputFrame* inputFrame) {

    static GameInputState lastState;

    GameInputState newState = lastState;
    newState.mousePosition = inputFrame->mousePosition;
    MouseEvents events;
    
    events.zoomChange = inputFrame->zoomChange;

    bool moved = false;

    // compare positions and check if moved
    events.mouseDelta = newState.mousePosition - lastState.mousePosition;
    f32 displacement = Magnitude(&events.mouseDelta);

    if(displacement > displacementThreshold) {
        moved = true;

        if (newState.middleButton == DOWN) {
            events.event = DRAG_CAMERA;
        }

        if(newState.leftButton == DOWN) {
            events.event = DRAG_ITEM;
        }
    }
    
    //printf("Mousepos %i, %i", inputFrame->mousePosition.x, inputFrame->mousePosition.y);

    events.mousePosition = inputFrame->mousePosition;

    // button down (start drag?)
    if(inputFrame->leftButton == 1) {
        newState.leftButton = DOWN;
    } 
    
    if (inputFrame->rightButton == 1) {
        newState.rightButton = DOWN;
    } 
    
    if (inputFrame->middleButton == 1) {
        newState.middleButton = DOWN;
    }

    // button up: clic
    if(inputFrame->leftButton == 2) {
        newState.leftButton = UP;
        if(!moved) events.event = CLIC;
    } 
    
    if (inputFrame->rightButton == 2) {
        newState.rightButton = UP;
        if(!moved) events.event = LEFT_CLIC;
    } 
    
    if (inputFrame->middleButton == 2) {
        newState.middleButton = UP;
    }

    lastState = newState;
    return events;
}
