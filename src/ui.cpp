
#include "ui.h"

void DrawWorldSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory, const int selectedBuildingId, float cameraZoom) {
    
    Building* selectedBuilding = QueryBuildingById(session->buildings, session->buildingCounter, selectedBuildingId);

    if(selectedBuilding != NULL) {
        DrawBuildingMenu (selectedBuilding, GetPosition (selectedBuilding, grid), mainFont);
    }
}

void DrawBuildingMenu (Building* selectedBuilding, Vector2 buildingWorldPosition, TTF_Font* mainFont) {

    Vector2 buildingScreenPosition = WorldToScreenPosition (buildingWorldPosition) + Vector2 { 0, selectedBuilding->roofHeight + 30 };

    BoundingBox panelBBox = { buildingScreenPosition.x, buildingScreenPosition.y, 150, 180 };
    DrawPanel(panelBBox, { 0, 0, 0, 200 });

    //printf("PanelBBox: [%f, %f, %f, %f]", panelBBox.x, panelBBox.y, panelBBox.width, panelBBox.height);
    RegisterButton(UIButton { panelBBox, PanelHit }, true);

    Vector2 origin = { panelBBox.x, panelBBox.y + panelBBox.height };
    int accumulatedYPadding = 0;
    Text text;
    int length;
    
    // Building name
    char textBuffer [64];
    length = sprintf(textBuffer, "%s", selectedBuilding->data.name);

    accumulatedYPadding += 30;
    CreateText(&text, textBuffer, mainFont);
    RenderText(&text, { origin.x, origin.y - accumulatedYPadding, 0, 30 });
    
    length = sprintf(textBuffer, "Energy: %i", selectedBuilding->data.io.energy);

    accumulatedYPadding += 40;
    CreateText(&text, textBuffer, mainFont);
    RenderText(&text, { origin.x, origin.y - accumulatedYPadding, 0, 20 });

    length = sprintf(textBuffer, "Produc: %i", selectedBuilding->data.io.production);

    accumulatedYPadding += 20;
    CreateText(&text, textBuffer, mainFont);
    RenderText(&text, { origin.x, origin.y - accumulatedYPadding, 0, 20 });

    length = sprintf(textBuffer, "Commer: %i", selectedBuilding->data.io.exchange);

    accumulatedYPadding += 20;
    CreateText(&text, textBuffer, mainFont);
    RenderText(&text, { origin.x, origin.y - accumulatedYPadding, 0, 20 }); 

    accumulatedYPadding += 50;
    float accumulatedXPadding = 10;

    BoundingBox buttonBox = BoundingBox {origin.x + accumulatedXPadding, origin.y - accumulatedYPadding, 40, 40 };
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(gameState->demolishIcon, buttonBox);
    RegisterButton({ buttonBox, PressDemolish }, true);

    buttonBox.x += 50;
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(gameState->upgradeIcon, buttonBox);
    RegisterButton (UIButton { buttonBox, PressUpgrade }, true);

    buttonBox.x += 50;
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(gameState->moveIcon, buttonBox);
    RegisterButton(UIButton { buttonBox, PressMove }, true);
}

void DrawMainPanel (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale) {

    int accumulatedYPadding = 0;
    Text* text;
    int length;
    // background
    BoundingBox panelBBox = TransformBox(BoundingBox {0, 0, 200, 200}, TOP_LEFT);
    DrawPanel(panelBBox, Color {0, 0, 0, 200 });
    RegisterButton(UIButton { panelBBox, PanelHit }, false);

    char textBuffer [64];
    length = sprintf(textBuffer, "Population: %i", session->population);

    text = (Text*)memory->temporaryStorageCurrent;
    memory->temporaryStorageCurrent += sizeof(Text*);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);
    
    length = sprintf(textBuffer, "Toxicity: %f", GetToxicity(session));

    text = (Text*)memory->temporaryStorageCurrent;
    memory->temporaryStorageCurrent += sizeof(Text*);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    accumulatedYPadding += 10;
    
    // resources header
    text = (Text*)memory->temporaryStorageCurrent;
    memory->temporaryStorageCurrent += sizeof(Text*);

    accumulatedYPadding += 30;
    CreateText(text, "RESOURCES", mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    // resources
    length = sprintf(textBuffer, "Energy: %i", session->resources.energy);

    text = (Text*)memory->temporaryStorageCurrent;
    memory->temporaryStorageCurrent += sizeof(Text*);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    length = sprintf(textBuffer, "Production: %i", session->resources.production);

    text = (Text*)memory->temporaryStorageCurrent;
    memory->temporaryStorageCurrent += sizeof(Text*);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    length = sprintf(textBuffer, "Exchange: %i", session->resources.exchange);

    text = (Text*)memory->temporaryStorageCurrent;
    memory->temporaryStorageCurrent += sizeof(Text*);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);
}

void DrawScreenSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory) {
    
    DrawMainPanel(session, mainFont, memory);
}

bool OnClicUI (Vector2 point, UIButton * buttons, int buttonCounter) {

    printf ("UI Clic at: %f, %f\n", point.x, point.y);

    for (int i = buttonCounter - 1; i >= 0; i--) {
        UIButton * button = buttons + i;
        if (CheckCollision(point, button->box)) {
            button->action();
            return true;
        }
    }

    return false;
}

BoundingBox TransformBox (BoundingBox box, UIAnchor anchor) {

    switch (anchor) {
        case TOP_LEFT : {
            return {
                box.x,
                windowDimension.height - box.y - box.height, 
                box.width,
                box.height,
            };
        }
    }

    return box;
}

void DrawPanel (BoundingBox box, Color color) {

    glColor4ub (color.r, color.g, color.b, color.a);

    glBegin(GL_QUADS);
        //top left
        glVertex3f( box.x, box.y, 0.5f);
        //top right
        glVertex3f( box.x + box.width, box.y, 0.5f);
        //bot right
        glVertex3f( box.x + box.width, box.y + box.height, 0.5f);
        //bot left
        glVertex3f( box.x, box.y + box.height, 0.5f);
    glEnd();

}

void RegisterButton (UIButton button, bool isWorldSpace) {

    // Transforms world
    if(isWorldSpace) {
        Vector2 screenPosition = WorldUIToScreenPosition( Vector2 { button.box.x, button.box.y });
        button.box = BoundingBox { screenPosition.x, screenPosition.y, button.box.width, button.box.height };
    }

    if(gameState->buttonCounter++ == 64) {
        printf("ERROR: no more than 64 buttons are allowed");
        return;
    }

    *(gameState->buttons + gameState->buttonCounter) = button;
    gameState->buttonCounter += 1;
}

Vector2 WorldUIToScreenPosition (Vector2 position) {
    Vector2 result = position + gameState->camera.actualTranslation;
    result = result * gameState->camera.actualZoom;
    result.x += windowDimension.width / 2;
    result.y += windowDimension.height / 2;
    return result;
}

void PressDemolish () {
    printf("Demolish!\n");

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);
    selectedBuilding->state = DESTROYED;

    gameState->selectedBuildingId = -1;
}

void PressUpgrade () {
    printf("Upgrade!\n");
}

void PressMove () {
    gameState->session.phase = GP_MOVING_BUILDING;
}

void PanelHit () {
    printf("PanelHit!\n");
}