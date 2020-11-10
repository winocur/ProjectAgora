
#include "ui.h"

void DrawWorldSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory, const int selectedBuildingId, float cameraZoom) {
    
    Building* selectedBuilding = QueryBuildingById(session->buildings, session->buildingCounter, selectedBuildingId);

    if(selectedBuilding != NULL && session->phase == GP_IDLE) {
        DrawBuildingMenu (selectedBuilding, GetPosition (selectedBuilding, grid), mainFont);
    }

}

void DrawBuildingMenu (Building* selectedBuilding, Vector2 buildingWorldPosition, TTF_Font* mainFont) {

    Vector2 buildingScreenPosition = WorldToScreenPosition (buildingWorldPosition) + Vector2 { 0, selectedBuilding->data.height + 30 };

    BoundingBox panelBBox = { buildingScreenPosition.x, buildingScreenPosition.y, 150, 230 };
    DrawPanel(panelBBox, { 0, 0, 0, 200 });

    //printf("PanelBBox: [%f, %f, %f, %f]", panelBBox.x, panelBBox.y, panelBBox.width, panelBBox.height);
    RegisterButton(UIButton { "Panel", panelBBox, [] () {}, NULL }, true);

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
    
    
    length = sprintf(textBuffer, "Toxicity: %f", GetBuildingToxicity(selectedBuilding));
    accumulatedYPadding += 40;
    CreateText(&text, textBuffer, mainFont);
    RenderText(&text, { origin.x, origin.y - accumulatedYPadding, 0, 20 });

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
    RegisterButton(UIButton { "Dismantle", buttonBox, PressDemolish, GetDemolisionCost }, true);

    buttonBox.x += 50;
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(gameState->upgradeIcon, buttonBox);
    RegisterButton (UIButton { "Upgrade", buttonBox, PressUpgrade, GetUpgradeCost }, true);

    buttonBox.x += 50;
    DrawPanel(buttonBox, {255,255,255,255});
    RenderSpriteToBox(gameState->moveIcon, buttonBox);
    RegisterButton(UIButton { "Move", buttonBox, PressMove, GetMoveCost }, true);
}

void DrawMainPanel (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale) {

    int accumulatedYPadding = 0;
    Text* text;
    int length;
    // background
    BoundingBox panelBBox = TransformBox(BoundingBox {0, 0, 200, 200}, TOP_LEFT);
    DrawPanel(panelBBox, Color {0, 0, 0, 200 });
    RegisterButton(UIButton { "Panel", panelBBox, [] () {} }, false);

    char textBuffer [64];
    length = sprintf(textBuffer, "Population: %i", session->population);

    text = GetFromTemporaryMemory<Text>(memory);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);
    
    length = sprintf(textBuffer, "Toxicity: %i", (int)GetToxicity(session));

    text = GetFromTemporaryMemory<Text>(memory);
    
    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    accumulatedYPadding += 10;
    
    // resources header
    text = GetFromTemporaryMemory<Text>(memory);

    accumulatedYPadding += 30;
    CreateText(text, "RESOURCES", gameState->titleFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    // resources
    length = sprintf(textBuffer, "Energy: %i", session->resources.energy);

    text = GetFromTemporaryMemory<Text>(memory);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    length = sprintf(textBuffer, "Production: %i", session->resources.production);

    text = GetFromTemporaryMemory<Text>(memory);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);

    length = sprintf(textBuffer, "Exchange: %i", session->resources.exchange);

    text = GetFromTemporaryMemory<Text>(memory);

    accumulatedYPadding += 30;
    CreateText(text, textBuffer, mainFont);
    RenderText(text, TransformBox({ 0, 0, 200, accumulatedYPadding }, TOP_LEFT), scale);
}

void DrawBuildButton () {

    BoundingBox buttonBox = TransformBox( { 10, 10, 100, 100 }, BOTTOM_RIGHT);
    DrawPanel(buttonBox, { 100, 100, 100, 100 } );
    RenderSpriteToBox(gameState->hammerIcon, buttonBox);
    RegisterButton(UIButton { "Build", buttonBox, [] () { gameState->session.phase = GP_BUILDING_MENU; }, NULL, [] () { return "Construct a new building"; } }, false);
}

void PressCreateBuilding (BuildingType type) {
    gameState->session.phase = GP_MOVING_BUILDING;
    Building building = CreateBuilding (type, 0, 0);
    building.state = MOVING;
    gameState->selectedBuildingId = PlaceBuilding(building)->instanceId;
}

void DrawCreateBuildingButton (BoundingBox rawBox, Sprite* sprite, void  (*clicAction)(void), char* buttonName) {
    auto box = TransformBox (rawBox, CENTER);
    DrawPanel (box, { 20, 20, 20, 255 });
    RenderSpriteToBox(sprite, box);
    RegisterButton(UIButton { buttonName, box, clicAction, GetDemolisionCost }, false);
}

void DrawBuildMenu () {

    DrawPanel(TransformBox ( { 0, 0, 500, 300 }, CENTER), { 100, 100, 100, 255 } );
    
    DrawCreateBuildingButton ({ -180, 0, 100, 100 }, gameState->housing,    [] () { PressCreateBuilding(BUILDING_BLOCKS); }, "Housing");
    DrawCreateBuildingButton ({ -60, 0, 100, 100 } , gameState->commercial, [] () { PressCreateBuilding(BUSINESS_OFFICES); }, "Commercial");
    DrawCreateBuildingButton ({ 60, 0, 100, 100 }  , gameState->power,      [] () { PressCreateBuilding(COAL_PLANT); }, "Power");
    DrawCreateBuildingButton ({ 180, 0, 100, 100 } , gameState->industry,   [] () { PressCreateBuilding(INDUSTRIAL_REFINERY); }, "Industry");
}

void DrawScreenSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory) {
    
    DrawTickTimer (session);
    DrawMainPanel(session, mainFont, memory);
    DrawBuildButton();
    DrawCostContainer (session, mainFont, memory);

    if(session->phase == GP_BUILDING_MENU) {
        DrawBuildMenu();
    }
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

bool OnHoverUI (Vector2 point, UIButton * buttons, int buttonCounter) {

    for (int i = buttonCounter - 1; i >= 0; i--) {
        UIButton * button = buttons + i;
        if (CheckCollision(point, button->box)) {
            gameState->hoverButton = button;
            return true;
        }
    }

    // we null the hover button to signal the UI not to draw cost panel
    gameState->hoverButton = NULL;
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
        
        case TOP_RIGHT : {
            return {
                windowDimension.width - box.x - box.width,
                windowDimension.height - box.y - box.height, 
                box.width,
                box.height,
            };
        }

        case CENTER : {
            return {
                (windowDimension.width / 2) - (box.width / 2) + box.x,
                (windowDimension.height / 2) - (box.height / 2) + box.y, 
                box.width,
                box.height
            };
        }

        case BOTTOM_CENTER : {
            return {
                (windowDimension.width / 2) - (box.width / 2) + box.x,
                box.y, 
                box.width,
                box.height,
            };
        }

        case BOTTOM_RIGHT : {
            return {
                windowDimension.width - box.x - box.width,
                box.y, 
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

void DrawCostContainer (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale) {

    if (gameState->hoverButton == NULL) return;

    const UIButton* button = gameState->hoverButton;

    if(button->costs == NULL && button->hoverMessage == NULL) return;
    
    BoundingBox panelBBox = TransformBox(BoundingBox {0,0,500,100} , BOTTOM_CENTER);
    DrawPanel(panelBBox, {100,0,0,150});
    
    Vector2 origin = { panelBBox.x, panelBBox.y + panelBBox.height };
    char textBuffer [64];


    if(button->costs != NULL) 
    {
        Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);
        
        Resources costs = button->costs(selectedBuilding);

        int accumulatedYPadding = 30;
        int xPadding = 15;
        
        DoText(button->name, { origin.x + xPadding, origin.y - accumulatedYPadding, 0, 40 }, gameState->titleFont);

        accumulatedYPadding += 20;

        int length = sprintf(textBuffer, "Energy: %i", costs.energy);

        DoText(textBuffer, { origin.x + xPadding, origin.y - accumulatedYPadding, 0, 30 }, mainFont);

        length = sprintf(textBuffer, "Produc: %i", costs.production);
        accumulatedYPadding += 20;

        DoText(textBuffer, { origin.x + xPadding, origin.y - accumulatedYPadding, 0, 20 }, mainFont);

        length = sprintf(textBuffer, "Commer: %i", costs.exchange);
        accumulatedYPadding += 20;

        DoText(textBuffer, { origin.x + xPadding, origin.y - accumulatedYPadding, 0, 20 }, mainFont);
    }
    else
    {
        DoText(button->hoverMessage(), { origin.x + 20, origin.y - 20, 0, 20 }, mainFont);
    }
}

void DrawTickTimer (GameSession* session) {
    f64 percentaje = session->timer / session->tickTime;

    DrawPanel(TransformBox({10,10,200,40 }, TOP_RIGHT), {100,0,0,230 });
    DrawPanel(TransformBox({10 + 200 - 200 * percentaje,15, 200 * percentaje, 30 }, TOP_RIGHT), {0,255,0,255 });
}

void PressDemolish () {
    printf("Demolish!\n");

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);

    if(DemolishBuilding(selectedBuilding)) {
        gameState->selectedBuildingId = -1;
    }
}

void PressUpgrade () {
    printf("Upgrade!\n");

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);

    if(UpgradeBuilding(selectedBuilding)) {
        gameState->selectedBuildingId = -1;
    }
}

void PressMove () {

    Building* selectedBuilding = QueryBuildingById(gameState->session.buildings, gameState->session.buildingCounter, gameState->selectedBuildingId);

    StartMovingBuilding(selectedBuilding);
}

void PressOpenBuildMenu () {
    gameState->session.phase = GP_BUILDING_MENU;
}

