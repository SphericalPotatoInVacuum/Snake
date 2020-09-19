#pragma once

#include "Ltexture.h"
#include "Utility.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

const int TILE_HEAD_UP = 0;
const int TILE_HEAD_DOWN = 1;
const int TILE_HEAD_LEFT = 2;
const int TILE_HEAD_RIGHT = 3;
const int TILE_TOP_LEFT = 4;
const int TILE_TOP_RIGHT = 5;
const int TILE_BOTTOM_LEFT = 6;
const int TILE_BOTTOM_RIGHT = 7;
const int TILE_STRAIGHT_UP = 8;
const int TILE_STRAIGHT_DOWN = 9;
const int TILE_STRAIGHT_LEFT = 10;
const int TILE_STRAIGHT_RIGHT = 11;
const int TILE_TAIL_UP = 12;
const int TILE_TAIL_DOWN = 13;
const int TILE_TAIL_LEFT = 14;
const int TILE_TAIL_RIGHT = 15;

struct SnakeSegment {
    SnakeSegment(int x, int y, int t) : position(x, y), type(t) {}
    Vector2i position;
    int type;
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction { None, Up, Down, Left, Right };

class Snake {
public:
    Snake(int lBlockSize);
    ~Snake();

    bool loadMedia(SDL_Renderer *lRenderer);

    // Helpers
    void setDirection(Direction lDir);
    Direction getDirection();
    Direction getPhysicalDirection();
    int getSpeed();
    Vector2i getPosition();
    int getLives();
    int getScore();
    void increaseScore();
    bool hasLost();

    void lose();
    void toggleLost();

    void extend();
    void reset(); // Reset to starting position

    void move();             // Movement method
    void updateTiles();      // Update tiles after moving
    void tick();             // Update method
    void cut(int lSegments); // Method for cutting snake
    void render(SDL_Renderer *lRenderer, float *scale);

private:
    void checkCollision(); // Check for collisions

    SnakeContainer mSnakeBody; // Segment vector
    int mSize;                 // Size of the graphics
    Direction mDir;            // Current direction
    int mSpeed;                // Current speed
    int mLives;                // Lives remaining
    int mScore;                // Current score
    bool mLost;                // Losing state
    SDL_Rect mSnakeClips[16];  // Texture handling
    LTexture mSnakeTextureSheet;
};