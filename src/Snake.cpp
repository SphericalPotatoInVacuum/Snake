#include "Snake.h"

Snake::Snake(int lBlockSize) {
    mSize = lBlockSize;
    reset();
}

Snake::~Snake() {
    mSnakeTextureSheet.free();
};

bool Snake::loadMedia(SDL_Renderer *lRenderer) {
    // Load sprite sheet texture
    if (!mSnakeTextureSheet.loadFromFile("Snake.png", lRenderer)) {
        printf("Failed to load apple texture!\n");
        return false;
    }

    mSnakeClips[TILE_HEAD_UP] = {0, 0, 16, 16};
    mSnakeClips[TILE_HEAD_DOWN] = {16, 0, 16, 16};
    mSnakeClips[TILE_HEAD_LEFT] = {0, 16, 16, 16};
    mSnakeClips[TILE_HEAD_RIGHT] = {16, 16, 16, 16};
    mSnakeClips[TILE_TOP_LEFT] = {32, 0, 16, 16};
    mSnakeClips[TILE_TOP_RIGHT] = {48, 0, 16, 16};
    mSnakeClips[TILE_BOTTOM_LEFT] = {32, 16, 16, 16};
    mSnakeClips[TILE_BOTTOM_RIGHT] = {48, 16, 16, 16};
    mSnakeClips[TILE_TAIL_UP] = {16, 32, 16, 16};
    mSnakeClips[TILE_TAIL_DOWN] = {0, 32, 16, 16};
    mSnakeClips[TILE_TAIL_LEFT] = {0, 48, 16, 16};
    mSnakeClips[TILE_TAIL_RIGHT] = {16, 48, 16, 16};
    mSnakeClips[TILE_STRAIGHT_UP] = {32, 32, 16, 16};
    mSnakeClips[TILE_STRAIGHT_DOWN] = {48, 32, 16, 16};
    mSnakeClips[TILE_STRAIGHT_LEFT] = {32, 48, 16, 16};
    mSnakeClips[TILE_STRAIGHT_RIGHT] = {48, 48, 16, 16};

    return true;
}

void Snake::reset() {
    mSnakeBody.clear();

    mSnakeBody.push_back(SnakeSegment(5, 7, 1));
    mSnakeBody.push_back(SnakeSegment(5, 6, 9));
    mSnakeBody.push_back(SnakeSegment(5, 5, 13));

    setDirection(Direction::None);
    mSpeed = 15;
    mLives = 3;
    mScore = 0;
    mLost = false;
}

void Snake::setDirection(Direction lDir) {
    mDir = lDir;
}
Direction Snake::getDirection() {
    return mDir;
}
Direction Snake::getPhysicalDirection() {
    if (mSnakeBody.size() < 2)
        return Direction::None;

    SnakeSegment &head = mSnakeBody[0];
    SnakeSegment &bone = mSnakeBody[1];

    if (head.position.x == bone.position.x) {
        return head.position.y > bone.position.y ? Direction::Down
                                                 : Direction::Up;
    } else if (head.position.y == bone.position.y) {
        return head.position.x > bone.position.x ? Direction::Right
                                                 : Direction::Left;
    }

    return Direction::None;
}
int Snake::getSpeed() {
    return mSpeed;
}
Vector2i Snake::getPosition() {
    return (!mSnakeBody.empty() ? mSnakeBody.front().position : Vector2i(1, 1));
}

int Snake::getLives() {
    return mLives;
}
int Snake::getScore() {
    return mScore;
}

void Snake::increaseScore() {
    mScore += 10;
}
bool Snake::hasLost() {
    return mLost;
}
void Snake::lose() {
    mLost = true;
}
void Snake::toggleLost() {
    mLost = !mLost;
}

void Snake::extend() {
    if (mSnakeBody.empty())
        return;
    SnakeSegment &tail_head = mSnakeBody[mSnakeBody.size() - 1];

    if (mSnakeBody.size() > 1) {
        SnakeSegment &tail_bone = mSnakeBody[mSnakeBody.size() - 2];
        if (tail_head.position.x == tail_bone.position.x) {
            if (tail_head.position.y > tail_bone.position.y) {
                mSnakeBody.push_back(SnakeSegment(tail_head.position.x,
                                                  tail_bone.position.y + 1,
                                                  TILE_TAIL_UP));
            } else {
                mSnakeBody.push_back(SnakeSegment(tail_head.position.x,
                                                  tail_bone.position.y - 1,
                                                  TILE_TAIL_DOWN));
            }
        } else if (tail_head.position.y == tail_bone.position.y) {
            if (tail_head.position.x > tail_bone.position.x) {
                mSnakeBody.push_back(SnakeSegment(tail_head.position.x + 1,
                                                  tail_bone.position.y,
                                                  TILE_TAIL_LEFT));
            } else {
                mSnakeBody.push_back(SnakeSegment(tail_head.position.x - 1,
                                                  tail_bone.position.y,
                                                  TILE_TAIL_RIGHT));
            }
        }
    } else {
        if (mDir == Direction::Up) {
            mSnakeBody.push_back(SnakeSegment(
                tail_head.position.x, tail_head.position.y + 1, TILE_TAIL_UP));
        } else if (mDir == Direction::Down) {
            mSnakeBody.push_back(SnakeSegment(tail_head.position.x,
                                              tail_head.position.y - 1,
                                              TILE_TAIL_DOWN));
        } else if (mDir == Direction::Left) {
            mSnakeBody.push_back(SnakeSegment(tail_head.position.x - 1,
                                              tail_head.position.y,
                                              TILE_TAIL_LEFT));
        } else if (mDir == Direction::Right) {
            mSnakeBody.push_back(SnakeSegment(tail_head.position.x + 1,
                                              tail_head.position.y,
                                              TILE_TAIL_RIGHT));
        }
    }

    updateTiles();
}

void Snake::tick() {
    if (mSnakeBody.empty())
        return;
    if (mDir == Direction::None)
        return;
    move();
    checkCollision();
}

void Snake::move() {
    for (int i = mSnakeBody.size() - 1; i > 0; i--) {
        mSnakeBody[i].position = mSnakeBody[i - 1].position;
    }
    if (mDir == Direction::Left) {
        mSnakeBody[0].position.x--;
    } else if (mDir == Direction::Right) {
        mSnakeBody[0].position.x++;
    } else if (mDir == Direction::Up) {
        mSnakeBody[0].position.y--;
    } else if (mDir == Direction::Down) {
        mSnakeBody[0].position.y++;
    }

    updateTiles();
}

void Snake::updateTiles() {
    if (mDir == Direction::Left) {
        mSnakeBody[0].type = TILE_HEAD_LEFT;
    } else if (mDir == Direction::Right) {
        mSnakeBody[0].type = TILE_HEAD_RIGHT;
    } else if (mDir == Direction::Up) {
        mSnakeBody[0].type = TILE_HEAD_UP;
    } else if (mDir == Direction::Down) {
        mSnakeBody[0].type = TILE_HEAD_DOWN;
    }

    Vector2i lDir1;
    Vector2i lDir2;
    for (int i = mSnakeBody.size() - 2; i > 0; i--) {
        lDir1 = mSnakeBody[i - 1].position - mSnakeBody[i].position;
        lDir2 = mSnakeBody[i + 1].position - mSnakeBody[i].position;

        if (lDir1 == Vector2i(0, -1) && lDir2 == Vector2i(0, 1)) {
            mSnakeBody[i].type = TILE_STRAIGHT_UP;
        } else if (lDir1 == Vector2i(0, 1) && lDir2 == Vector2i(0, -1)) {
            mSnakeBody[i].type = TILE_STRAIGHT_DOWN;
        } else if (lDir1 == Vector2i(-1, 0) && lDir2 == Vector2i(1, 0)) {
            mSnakeBody[i].type = TILE_STRAIGHT_LEFT;
        } else if (lDir1 == Vector2i(1, 0) && lDir2 == Vector2i(-1, 0)) {
            mSnakeBody[i].type = TILE_STRAIGHT_RIGHT;
        }
        if (lDir1 == Vector2i(1, 0) && lDir2 == Vector2i(0, 1) ||
            lDir1 == Vector2i(0, 1) && lDir2 == Vector2i(1, 0)) {
            mSnakeBody[i].type = TILE_TOP_LEFT;
        } else if (lDir1 == Vector2i(-1, 0) && lDir2 == Vector2i(0, 1) ||
                   lDir1 == Vector2i(0, 1) && lDir2 == Vector2i(-1, 0)) {
            mSnakeBody[i].type = TILE_TOP_RIGHT;
        } else if (lDir1 == Vector2i(0, -1) && lDir2 == Vector2i(1, 0) ||
                   lDir1 == Vector2i(1, 0) && lDir2 == Vector2i(0, -1)) {
            mSnakeBody[i].type = TILE_BOTTOM_LEFT;
        } else if (lDir1 == Vector2i(0, -1) && lDir2 == Vector2i(-1, 0) ||
                   lDir1 == Vector2i(-1, 0) && lDir2 == Vector2i(0, -1)) {
            mSnakeBody[i].type = TILE_BOTTOM_RIGHT;
        }
    }

    SnakeSegment &tail = mSnakeBody.rbegin()[0];
    SnakeSegment &bone = mSnakeBody.rbegin()[1];
    lDir1 = bone.position - tail.position;
    if (lDir1 == Vector2i(0, -1)) {
        tail.type = TILE_TAIL_UP;
    } else if (lDir1 == Vector2i(0, 1)) {
        tail.type = TILE_TAIL_DOWN;
    } else if (lDir1 == Vector2i(-1, 0)) {
        tail.type = TILE_TAIL_LEFT;
    } else if (lDir1 == Vector2i(1, 0)) {
        tail.type = TILE_TAIL_RIGHT;
    }
}

void Snake::checkCollision() {
    if (mSnakeBody.size() < 5)
        return;
    SnakeSegment &head = mSnakeBody.front();
    for (auto itr = mSnakeBody.begin() + 1; itr != mSnakeBody.end(); itr++) {
        if (itr->position == head.position) {
            int segments = mSnakeBody.end() - itr;
            cut(segments);
            break;
        }
    }
}

void Snake::cut(int l_segments) {
    for (int i = 0; i < l_segments; i++) {
        mSnakeBody.pop_back();
    }
    mLives--;
    if (!mLives) {
        lose();
        return;
    }
    updateTiles();
}

void Snake::render(SDL_Renderer *lRenderer, float *scale) {
    if (mSnakeBody.empty())
        return;

    auto head = mSnakeBody.begin();
    mSnakeTextureSheet.render(head->position.x * mSize,
                              head->position.y * mSize, lRenderer,
                              &mSnakeClips[head->type], scale);

    for (auto itr = mSnakeBody.begin() + 1; itr != mSnakeBody.end(); itr++) {
        mSnakeTextureSheet.render(itr->position.x * mSize,
                                  itr->position.y * mSize, lRenderer,
                                  &mSnakeClips[itr->type], scale);
    }
}