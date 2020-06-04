#ifndef H_UI
#define H_UI

struct UIButton {
    BoundingBox box;

    void  (*action)(void);
    Resources (*costs)(const Building*);
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

BoundingBox TransformBox (BoundingBox box, UIAnchor anchor);


void RegisterButton (UIButton button, bool isWorldSpace);
Vector2 WorldUIToScreenPosition (Vector2 position);

void PressDemolish ();
void PressUpgrade ();
void PressMove ();
void PanelHit ();

#endif
