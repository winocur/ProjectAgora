#ifndef H_UI
#define H_UI

enum UIAnchor {
    TOP_LEFT,
    TOP_RIGHT,
    CENTER,
    BOTTOM_CENTER,
    BOTTOM_RIGHT,
};


struct UIButton {
    char* name;
    BoundingBox box;

    void  (*action)(void);
    Resources (*costs)(const Building*);
    const char* (*hoverMessage)();
};

bool OnClicUI (Vector2 point, UIButton * buttons, int buttonCounter);

bool OnHoverUI (Vector2 point, UIButton * buttons, int buttonCounter);

void DrawMainPanel (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale = 1);

void DrawScreenSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory) ;

void DrawWorldSpaceUI (GameSession* session, TTF_Font* mainFont, GameMemory* memory, int selectedBuildingId, float cameraZoom) ;

void DrawPanel (BoundingBox box, Color color);

void DrawCostContainer (GameSession* session, TTF_Font* mainFont, GameMemory* memory, float scale = 1);

void DrawTickTimer (GameSession* session);

void DrawBuildingMenu (Building* selectedBuilding, Vector2 buildingWorldPosition, TTF_Font* mainFont);

void DrawBuildButton () ;

const char* GetBuildMessage () ;

BoundingBox TransformBox (BoundingBox box, UIAnchor anchor);


void RegisterButton (UIButton button, bool isWorldSpace);
Vector2 WorldUIToScreenPosition (Vector2 position);

void PressDemolish ();
void PressUpgrade ();
void PressMove ();
void PanelHit ();
void PressOpenBuildMenu ();


#endif
