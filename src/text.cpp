#include "text.h"


Text* CreateText(Text* text, const char* message, TTF_Font* font)
{
    GLenum type = GL_BGRA;
    
    SDL_Color color = { 255, 255, 255, 255 };
    text->content = message;

    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
    
    if (surf == NULL) {
        TTF_CloseFont(font);
        printf("Failed rendering TTF");
        return NULL;
    }

    glGenTextures(1, &text->texture);
    glBindTexture(GL_TEXTURE_2D, text->texture);

    text->width  = surf->w;
    text->height = surf->h;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->width,
          text->height, 0, type,
          GL_UNSIGNED_BYTE,
          surf->pixels);

    //TODO error handling
    if(glGetError() != 0) {
        printf("OpenGL error");
    }

    /* Linear Filtering */
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //Clean up the surface and font
    SDL_FreeSurface(surf);
    return text;
}

void RenderText (Text* text, BoundingBox box, float scale) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, text->texture);
    glColor4ub (255, 255, 255, 255);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex3f(box.x, box.y, 0);
      
        glTexCoord2f(1, 1);
        glVertex3f(box.x + text->width, box.y, 0);
        
        glTexCoord2f(1, 0); 
        glVertex3f(box.x + text->width, box.y + text->height, 0);
        
        glTexCoord2f(0, 0);
        glVertex3f(box.x, box.y + text->height, 0);
        
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

TTF_Font* LoadFont (char* fontFile, int fontSize) {
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile, fontSize);
    if (font == NULL) {
        printf("No font %s\n", fontFile);
        return NULL;
    }
}

void DoText (const char* message, BoundingBox box, TTF_Font* font, float scale) {
    Text textMem;
    Text* text = CreateText(&textMem, message, font);
    RenderText(text, box, scale);
}