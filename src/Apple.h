#pragma once

#include "Constants.h"
#include "Ltexture.h"
#include "Utility.h"
#include <SDL2/SDL.h>

class Apple {
  public:
    Apple();
    ~Apple();

    bool loadMedia(SDL_Renderer *lRenderer);

    Vector2i getPosition();

    void respawn();
    void render(SDL_Renderer *lRenderer, float *scale);

  private:
    Vector2i mPosition;

    LTexture mAppleTexture;
};