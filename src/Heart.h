#pragma once

#include "Constants.h"
#include "Ltexture.h"
#include "Ltimer.h"
#include "Utility.h"
#include <SDL2/SDL.h>

class Heart {
  public:
    Heart(int x, int y);
    ~Heart();

    bool loadMedia(SDL_Renderer *lRenderer);

    void render(SDL_Renderer *lRenderer, float *scale);

    static void update();

  private:
    Vector2i mPosition;

    static const int FRAME_DURATION = 400;
    static const int FRAME_COUNT = 2;

    static LTimer mFrameTimer;
    static int mFrame;
    SDL_Rect mHeartClips[2];
    LTexture mHeartTextureSheet;
};