#pragma once

#define STATIC

#define VK_SPACE 0x20
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_ESCAPE 0x1B

enum SnakeDirection { UP, DOWN, LEFT, RIGHT };

const float SNAKE_TILE_SIZE = 25.f;
const float SECONDS_BETWEEN_DEATH_AND_EXIT = 1.5f;