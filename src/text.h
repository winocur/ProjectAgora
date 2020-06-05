#ifndef H_TEXT
#define H_TEXT
 
struct Text {
    const char * content;
    int width;
    int height;
    Color color;
    GLuint texture;
};

void RenderText (Text* text, BoundingBox box, float scale = 1);

Text* CreateText(Text* text, const char* message, TTF_Font* font);

void DoText (const char* message, BoundingBox box, TTF_Font* font, float scale = 1);
#endif