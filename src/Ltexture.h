#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// Texture wrapper class
class LTexture {
public:
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path, SDL_Renderer *lRenderer);

    // Loads image from specified string
    bool loadFromRenderedText(std::string textureText, TTF_Font *lFont, SDL_Renderer *lRenderer, SDL_Color textColor);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int x, int y, SDL_Renderer *lRenderer, SDL_Rect *clip = NULL, float *scale = NULL);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};
