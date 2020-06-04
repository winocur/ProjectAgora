#ifndef H_TEXT
#define H_TEXT
 
enum UIAnchor {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_CENTER
};

struct Text {
    const char * content;
    int width;
    int height;
    Color color;
    GLuint texture;
};

void RenderText (Text* text, BoundingBox box, float scale = 1);

Text* CreateText(Text* text, const char* message, TTF_Font* font);

#endif