#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL_opengl.h>

#include "agora.h"

WindowDimension windowDimension;

#include "sprites.cpp"
#include "input.cpp"
#include "text.cpp"
#include "grid.cpp"

#include "resources.h"
#include "buildings.cpp"
#include "simulation.cpp"
#include "ui.cpp"

void TestButton () {
    printf("Test button pressed!\n");
}

void GameInit(SDL_Surface * windowSurface, GameMemory * gameMemory) {


    //initialize game memory
    GameState * gs = (GameState *)gameMemory->permanentStorage;
    gs->camera.targetZoom = 1;
    gs->camera.actualZoom = 1;
    gameState = gs;
    
    // load icons
    SpriteSheet* sheet = LoadSpriteSheet (&gs->spriteSheets[gs->nextSpriteSheet++], "../assets/images/demolish.png", GL_RGBA, PNG, windowSurface, 1,1);
    gs->demolishIcon = LoadSprite(&gs->_sprites[gs->nextSprite++],sheet, "demolish", 0, 0);     

    
    sheet = LoadSpriteSheet (&gs->spriteSheets[gs->nextSpriteSheet++], "../assets/images/move.png", GL_RGBA, PNG, windowSurface, 1,1);
    gs->moveIcon = LoadSprite(&gs->_sprites[gs->nextSprite++], sheet, "move", 0, 0);

    sheet = LoadSpriteSheet (&gs->spriteSheets[gs->nextSpriteSheet++], "../assets/images/upgrade.png", GL_RGBA, PNG, windowSurface, 1,1);
    gs->upgradeIcon = LoadSprite(&gs->_sprites[gs->nextSprite++], sheet, "upgrade", 0, 0);

    
    // create grid
    PopulateGrid(&gs->grid);
    grid = &gs->grid;

    // load fonts
    gs->mainFont = LoadFont("../assets/fonts/sansation/Sansation-Regular.ttf", 15);
    gs->selectedBuildingId = -1;
    gs->session = StartGame();
}

void CenterCamera (Building* building) {
    gameState->camera.targetTranslation = ScreenToProjection(GetPosition (building, grid)) * -1 - Vector2 { 0, building->roofHeight };
}

void GameUpdateAndRender (GameMemory * gameMemory, WindowDimension windowDim, f64 msElapsed, GameInputFrame input) {

    
    windowDimension = windowDim;

    GLenum error = GL_NO_ERROR;

    float deltaTime = msElapsed / 1000.0;

    // input
    MouseEvents mouseEvents = ProcessMouseInput(&input);
    GameState * gs = (GameState *)gameMemory->permanentStorage;
    gameState = gs;

    Camera* camera = &gs->camera;

    camera->targetZoom += mouseEvents.zoomChange;
    if(camera->targetZoom > 2) camera->targetZoom = 2;
    if (camera->targetZoom < 0.5) camera->targetZoom = 0.5f;

    Vector2 worldMousePosition = mouseEvents.mousePosition;
    worldMousePosition = worldMousePosition - Vector2 { windowDimension.width / 2, windowDimension.height / 2 };
    worldMousePosition = worldMousePosition * camera->actualZoom;
    worldMousePosition = worldMousePosition - camera->actualTranslation;
    
    if(mouseEvents.event == CLIC) {
        if(!OnClicUI(mouseEvents.mousePosition, gs->buttons, gs->buttonCounter)) {

            Tile selected = QueryTile (&gs->grid, worldMousePosition);
            gs->grid.selectedX = selected.indexX;
            gs->grid.selectedY = selected.indexY;

            // select a building
            Building* touched = QueryBuilding (&selected, gs->session.buildings, gs->session.buildingCounter);

            // unselect previous building
            if(gs->selectedBuildingId != -1) {
                Building* selectedBuilding = QueryBuildingById (gs->session.buildings, gs->session.buildingCounter, gs->selectedBuildingId);
                selectedBuilding->state = IDLE;
            }

            if(touched != NULL) {
                printf("Touched building: %s [%i,%i]\n", touched->data.name, touched->gridX, touched->gridY);
                touched->state = SELECTED;
                CenterCamera(touched);
                gs->selectedBuildingId = touched->instanceId;
            } else {
                gs->selectedBuildingId = -1;
            }
        }
    } else if (mouseEvents.event == LEFT_CLIC) {
        OutputDebug("Left Clic");    
    } else if(mouseEvents.event == DRAG_CAMERA) {
        camera->targetTranslation = camera->targetTranslation + mouseEvents.mouseDelta * camera->actualZoom;
    } else if (mouseEvents.event == DRAG_ITEM) {

        Tile selected = QueryTile (&gs->grid, worldMousePosition);

        //gs->selectedBuilding->gridX = selected.indexX;
        //gs->selectedBuilding->gridY = selected.indexY;
    }

    error = glGetError();
    if(error != GL_NO_ERROR) {
        printf("OpenGL Error: %s\n", gluErrorString( error ));
    }

    // clears temporary memory in frame bound
    gameMemory->temporaryStorageCurrent = gameMemory->temporaryStorage; 
    gameState->buttons = (UIButton*)gameMemory->temporaryStorageCurrent;
    gameState->buttonCounter = 0;
    gameMemory->temporaryStorageCurrent += sizeof(UIButton) * 64;

    // camera interp
    camera->actualZoom = Lerp(camera->actualZoom, camera->targetZoom, 10     * deltaTime);
    camera->actualTranslation = Lerp(camera->actualTranslation, camera->targetTranslation, 8 * deltaTime);

    // simulation
    gs->session.timer += msElapsed;
    if (gs->session.timer > gs->session.tickTime) {
        ProcessTick(&gs->session);
    }

    // --  rendering --
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Pop default matrix onto current matrix
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    //Save default matrix again with camera translation
    glPushMatrix();

    Building sortedBuildings[32];
    SortBuildings(gs->session.buildings, sortedBuildings, gs->session.buildingCounter);

    glPushMatrix();

    // world space projection
    glOrtho( -windowDimension.width * camera->actualZoom / 2, windowDimension.width * camera->actualZoom / 2, -windowDimension.height * camera->actualZoom / 2, windowDimension.height * camera->actualZoom / 2, 1.0, -1.0 );
    glTranslatef(camera->actualTranslation.x,camera->actualTranslation.y, 0);  
      

    Tile hover = QueryTile (&gs->grid, worldMousePosition);
    HighlightTile(hover, { 0, 120, 120, 120 });

    RenderGrid (&gs->grid);

    for (int i = 0; i < gs->session.buildingCounter; i++) {
        
        // skip destroyed buildings
        if(sortedBuildings[i].state == DESTROYED) continue;

        RenderBuilding(&sortedBuildings[i], &gs->grid);
    }

    glPopMatrix();

    // World space UI
    glPushMatrix();
    glOrtho( -windowDimension.width / 2, windowDimension.width / 2, -windowDimension.height / 2, windowDimension.height / 2, 1.0, -1.0 );

    glTranslatef(camera->actualTranslation.x / camera->actualZoom, camera->actualTranslation.y / camera->actualZoom, 0);  

    DrawWorldSpaceUI(&gs->session, gs->mainFont, gameMemory, gs->selectedBuildingId, gs->camera.actualZoom);
    glPopMatrix();

    // screen space UI
    glOrtho( 0, windowDimension.width, 0, windowDimension.height, 1.0, -1.0 );
    DrawScreenSpaceUI(&gs->session, gs->mainFont, gameMemory) ;

    
}

void GameCleanup(GameMemory * gm) {
    free (gm->permanentStorage);
    free (gm->temporaryStorage);
}

Vector2 WorldToScreenPosition (Vector2 worldPosition) {
    Camera camera = gameState->camera;
    Vector2 projectedPosition = ScreenToProjection(worldPosition);
    return Vector2 { projectedPosition.x / camera.actualZoom, projectedPosition.y / camera.actualZoom };
}

