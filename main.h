#ifndef MAIN_H
#define MAIN_H
#include "math.h"
#include "stdlib.h"
#include "time.h"
#include <raylib.h>
#include <time.h>
// Global constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_COLLIDERS 1000
#define NUM_SPAWNER 10
#define ENEMY_SPAWN_TIME 2.0f
#define NUM_ENEMIES 1000
#define NUM_BOUNDS 4
typedef struct Player {
  Vector2 playerPos;
  Vector2 playerVel;
  Image playerImg;
  Texture2D playerTex;
  Rectangle playerRect;
} player;
typedef struct Attack {
  Vector2 attackPos;
  Texture2D explosion;
  Rectangle frameRec;
  int totalFrames;
  int currentFrame;
  int frameCounter;
  int frameSpeed;
} attack;
typedef struct Enemy {
  Vector2 enemyPos;
  Rectangle enemyBod;
  Color enemyCol;
  bool isActive;
} enemies;
typedef struct Spawner {
  Rectangle spawnerBod;
  Vector2 spawnerPos;
  Color spawnerCol;
} spawners;
typedef struct worldBounds {
  Rectangle upRec;
  Rectangle downRec;
  Rectangle rightRec;
  Rectangle leftRec;
} bounds;
#endif
