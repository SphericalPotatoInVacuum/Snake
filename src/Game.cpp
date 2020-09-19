#include "Game.h"

Game::Game() : mSnake(BLOCK_SIZE), mIsDone(false), tickTimer(), fpsTimer() {
    mScale = new float(BLOCK_SIZE / 16.0f);
    mHearts.clear();
    for (int i = 0; i < 3; i++) {
        mHearts.push_back(Heart(i, 0));
    }
    mScoreClip =
        new SDL_Rect({0, 0, SCREEN_WIDTH - 3 * BLOCK_SIZE, BLOCK_SIZE});
    tickTimer.start();
    fpsTimer.start();
}

bool Game::init() {

    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
        printf("Warning: Linear texture filtering not enabled!");
    }

    // Create window
    mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
    }

    // Create vsynced renderer for window
    mRenderer = SDL_CreateRenderer(
        mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n",
               SDL_GetError());
        success = false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n",
               IMG_GetError());
        success = false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
               Mix_GetError());
        success = false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
               TTF_GetError());
        success = false;
    }

    return success;
}

bool Game::loadMedia() {

    bool success;

    // Load sprite sheet texture
    if (!mApple.loadMedia(mRenderer)) {
        success = false;
    }

    if (!mSnake.loadMedia(mRenderer)) {
        success = false;
    }

    for (Heart &heart : mHearts) {
        if (!heart.loadMedia(mRenderer)) {
            success = false;
        }
    }

    // Load font
    mScoreFont = TTF_OpenFont("ArcadeClassic.ttf", 28);
    if (mScoreFont == NULL) {
        printf("Failed to font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    // Load sound effects
    mCollectSound = Mix_LoadWAV("collect.ogg");
    if (mCollectSound == NULL) {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n",
               Mix_GetError());
        success = false;
    }

    return success;
}

void Game::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            mIsDone = true;
        }
        // User presses a key
        else if (e.type == SDL_KEYDOWN) {
            // Select surfaces based on key press
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (mSnake.getPhysicalDirection() != Direction::Down) {
                    mSnake.setDirection(Direction::Up);
                }
                break;
            case SDLK_DOWN:
                if (mSnake.getPhysicalDirection() != Direction::Up) {
                    mSnake.setDirection(Direction::Down);
                }
                break;
            case SDLK_LEFT:
                if (mSnake.getPhysicalDirection() != Direction::Right) {
                    mSnake.setDirection(Direction::Left);
                }
                break;
            case SDLK_RIGHT:
                if (mSnake.getPhysicalDirection() != Direction::Left) {
                    mSnake.setDirection(Direction::Right);
                }
                break;
            }
        }
    }
}

void Game::update() {
    Uint32 timestep = 1000 / mSnake.getSpeed();
    if (tickTimer.getTicks() >= timestep) {
        mSnake.tick();
        if (mSnake.getPosition() == mApple.getPosition()) {
            mSnake.extend();
            mSnake.increaseScore();
            Mix_PlayChannel(-1, mCollectSound, 0);
            mApple.respawn();
        }
        int gridSize_x = SCREEN_WIDTH / BLOCK_SIZE;
        int gridSize_y = SCREEN_HEIGHT / BLOCK_SIZE;
        if (mSnake.getPosition().x < 0 || mSnake.getPosition().y < 0 ||
            mSnake.getPosition().x >= gridSize_x ||
            mSnake.getPosition().y >= gridSize_y) {
            mSnake.lose();
        }
        tickTimer.start();
        if (mSnake.hasLost()) {
            mSnake.reset();
        }
    }

    // Update Heart animation frame
    Heart::update();

    // Update score texture
    mScoreText.loadFromRenderedText("Score " +
                                        std::to_string(mSnake.getScore()),
                                    mScoreFont, mRenderer, {0, 0, 0});
    mScoreClip->w =
        mScoreText.getWidth() * ((BLOCK_SIZE + 0.0f) / mScoreText.getHeight());
}

void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);

    mSnake.render(mRenderer, mScale);
    mApple.render(mRenderer, mScale);
    for (int i = 0; i < mSnake.getLives(); i++) {
        mHearts[i].render(mRenderer, mScale);
    }
    mScoreText.render(3 * BLOCK_SIZE, 0, mRenderer, mScoreClip);

    // Update screen
    SDL_RenderPresent(mRenderer);
    printf("\rFPS: %f", 1000.0f / fpsTimer.getTicks());
    fpsTimer.start();
}

void Game::close() {
    // Destroy window
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
    mRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    Mix_Quit();
}

bool Game::isDone() {
    return mIsDone;
}