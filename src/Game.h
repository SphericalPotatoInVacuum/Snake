#pragma once

#include "Apple.h"
#include "Constants.h"
#include "Heart.h"
#include "Ltexture.h"
#include "Ltimer.h"
#include "Snake.h"
#include "Utility.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Game {
public:
    Game();
    bool init();
    bool loadMedia();
    void handleInput();
    void update();
    void render();
    void close();

    SDL_Window *GetWindow();
    bool isDone();

    void AddMessage(std::string lMessage);

private:
    Snake mSnake;
    Apple mApple;
    std::vector<Heart> mHearts;

    SDL_Window *mWindow = NULL;     // The window we'll be rendering to
    SDL_Renderer *mRenderer = NULL; // The window renderer
    float *mScale;

    LTexture mScoreText;
    TTF_Font *mScoreFont = NULL;
    SDL_Rect *mScoreClip = NULL;

    Mix_Chunk *mCollectSound = NULL;
    Mix_Chunk *mHitSound = NULL;

    LTimer tickTimer;
    LTimer fpsTimer;

    bool mIsDone;
};