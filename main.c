#include "include/raylib.h"
#include <stdio.h>
#include <time.h>
#include "math.h"
#include "fcntl.h"
#include "time.h"
#include "stdlib.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_COLLIDERS 1000
#define NUM_SPAWNER 10
#define ENEMY_SPAWN_TIME 2.0f
#define NUM_ENEMIES 1000
#define NUM_BOUNDS 4
Camera2D camera; //camera
struct Player
{
  Vector2 playerPos;
  Vector2 playerVel;
  Image playerImg;
  Texture2D playerTex;
  Rectangle playerRect;
};
struct Attack
{
  Vector2 attackPos;
  Texture2D explosion;
  Rectangle frameRec;
  int totalFrames;
  int currentFrame;
  int frameCounter;
  int frameSpeed;
};
typedef struct Enemy
{
  Vector2 enemyPos;
  Rectangle enemyBod;
  Color enemyCol;
  bool isActive;
}enemies;
typedef struct Spawner  
{
  Rectangle spawnerBod;
  Vector2 spawnerPos;
  Color spawnerCol;
}spawners;
typedef  struct worldBounds
{
  Rectangle upRec;
  Rectangle downRec;
  Rectangle rightRec;
  Rectangle leftRec;
}bounds;
struct Player player;
struct Attack attack;
spawners spawnPoint[NUM_SPAWNER];
bounds walls;
void initSpawner()
{
  for (int i = 0 ; i < NUM_SPAWNER; i++) 
  {
    float x = rand() % 10000;
    float y = rand() % 10000;
    spawnPoint[i].spawnerBod.x = x;
    spawnPoint[i].spawnerBod.y = y;
    spawnPoint[i].spawnerBod.width = 90;
    spawnPoint[i].spawnerBod.height = 90;
    spawnPoint[i].spawnerPos = (Vector2){x,y};
    //this is just a debug block 
    /*spawnPoint[i].spawnerBod.height = 20;*/
    /*spawnPoint[i].spawnerBod.width = 20;*/
    /*spawnPoint[i].spawnerCol = WHITE;*/
    //upto here 
    //make sure spawners are spawned far apart
    
  }
}
//draw spawners for debug
void drawSpawner()
{
  for (int i = 0 ; i < NUM_SPAWNER; i++)
  {
    DrawRectangleRec(spawnPoint[i].spawnerBod, WHITE);
  /*DrawLine(spawnPoint[i].spawnerPos.x,spawnPoint[i].spawnerPos.y,player.playerPos.x,player.playerPos.y,WHITE);*/
  }
}
float hordeTimer = 0.0f;
enemies enemy[NUM_ENEMIES];
float enemySpawnTimer = 0.0f;
void spawnEnemy()
{
  for (int i = 0 ; i < NUM_ENEMIES; i++) 
  {
    if(!enemy[i].isActive)
    {
      int spawnIndex = rand() % NUM_SPAWNER;
      enemy[i].enemyBod = spawnPoint[spawnIndex].spawnerBod;
      enemy[i].isActive = true;
      enemy[i].enemyBod.height =10;
      enemy[i].enemyBod.width =10;
      enemy[i].enemyCol = RED;
      return;
    }
  }
}
void worldBounds()
{
  walls.downRec = (Rectangle){-100,1000,1500,10};
  walls.upRec = (Rectangle){-100,-1000,1500,10};
  walls.leftRec = (Rectangle){-100,-1000,10,2000};
  walls.rightRec = (Rectangle){1400,-1000,10,2000};
  DrawRectangleRec(walls.downRec, WHITE);
  DrawRectangleRec(walls.upRec, WHITE);
  DrawRectangleRec(walls.leftRec, WHITE);
  DrawRectangleRec(walls.rightRec, WHITE);
  //debug
  DrawLine(player.playerPos.x,player.playerPos.y,walls.upRec.x,walls.upRec.y,WHITE);
  DrawLine(player.playerPos.x,player.playerPos.y,walls.leftRec.x,walls.leftRec.y,WHITE);
  DrawLine(player.playerPos.x,player.playerPos.y,walls.downRec.x,walls.downRec.y,WHITE);
  DrawLine(player.playerPos.x,player.playerPos.y,walls.rightRec.x,walls.rightRec.y,WHITE);
  //check CheckCollisionRecs
  if(CheckCollisionRecs(walls.downRec,player.playerRect))
  {
    player.playerPos.y -= 10; 
  }
  if(CheckCollisionRecs(walls.leftRec,player.playerRect))
  {
    player.playerPos.x += 10;
  }
  if(CheckCollisionRecs(walls.upRec,player.playerRect))
  {
    player.playerPos.y += 10; 
  }
  if(CheckCollisionRecs(walls.rightRec,player.playerRect))
  {
    player.playerPos.x -= 10;
  }
}
void Enemy ()
{
  Vector2 attackPos = GetScreenToWorld2D(GetMousePosition(), camera);
  Vector2 attacCen = {attackPos.x - player.playerPos.x + player.playerPos.x,attackPos.y - player.playerPos.y + player.playerPos.y};
  for(int i =0;i < NUM_ENEMIES;i++)
  {
    if(!enemy[i].isActive) continue;
    Vector2 direction = 
    {
      player.playerPos.x - enemy[i].enemyBod.x,
      player.playerPos.y - enemy[i].enemyBod.y
    };
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if(length > 0)
    {
      direction.x /= length;
      direction.y /= length;
    }
    float enemySpeed = 5.0f;
    enemy[i].enemyBod.x += direction.x * enemySpeed;
    enemy[i].enemyBod.y += direction.y * enemySpeed;
    for(int j =0;j < NUM_ENEMIES;j++)
    {
      if(i != j && enemy[i].isActive)
      {
        float dx = enemy[i].enemyBod.x - enemy[j].enemyBod.x;
        float dy = enemy[i].enemyBod.y - enemy[j].enemyBod.y;
        float distance = sqrtf(dx * dx + dy * dy);
        if(distance < 50.0f)
        {
          enemy[i].enemyBod.x += dx * 0.05f;
          enemy[i].enemyBod.y += dy * 0.05f;
        }
      }
    }
    if(CheckCollisionCircleRec(attacCen, 20 , enemy[i].enemyBod))
    {
      DrawCircle(attacCen.x, attacCen.y, 20 , RED);
      if(IsMouseButtonDown(0))
      {
        enemy[i].isActive = false;  
        camera.offset = (Vector2){(float)SCREEN_WIDTH/2 + 30,(float)SCREEN_HEIGHT/2}; 
      }
    }
    if(IsKeyDown(KEY_Q))
    {
      DrawLine(player.playerPos.x, player.playerPos.y, enemy[i].enemyBod.x, enemy[i].enemyBod.y , RED);
    }
  }
} 
void drawEnemy()
{
  for (int i =0 ; i < NUM_ENEMIES; i++) 
  {
    if(enemy[i].isActive)
    {
      DrawRectangleRec(enemy[i].enemyBod, enemy[i].enemyCol);
      /*DrawLine(enemy[i].enemyBod.x, enemy[i].enemyBod.y, player.playerPos.x, player.playerPos.y, WHITE); ///DEBUG thing*/
    }
  }
}
void updatePlayer()
{
  /*Vector2 screenPos = GetScreenToWorld2D(player.playerPos, camera);*/
  player.playerRect = (Rectangle){player.playerPos.x,player.playerPos.y,20 ,20};
  for(int i = 0; i < NUM_ENEMIES; i++)
  {
    DrawRectangleLinesEx(player.playerRect, 2, WHITE);
   if(CheckCollisionRecs(player.playerRect, enemy[i].enemyBod))
    {
      player.playerTex.height = 0;
      player.playerTex.width = 0;
      ClearBackground(BLACK);
    }
  }
}
void input() 
{
  player.playerVel = (Vector2){500.0f,500.0f};
  if(IsKeyDown(KEY_D) && player.playerTex.width != 0) player.playerPos.x += player.playerVel.x * GetFrameTime();
  if(IsKeyDown(KEY_A) && player.playerTex.width !=0) player.playerPos.x -= player.playerVel.x * GetFrameTime();
  if(IsKeyDown(KEY_W) && player.playerTex.width != 0) player.playerPos.y -= player.playerVel.y * GetFrameTime();
  if(IsKeyDown(KEY_S) && player.playerTex.width != 0) player.playerPos.y += player.playerVel.y * GetFrameTime();
  //debugging
  if(IsKeyPressed(KEY_F)) ToggleFullscreen();
  if(IsKeyPressed(KEY_F1)) HideCursor();
  Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
  float dx = mousePos.x - player.playerPos.x;
  float dy = mousePos.y - player.playerPos.y;
  float rotation = atan2f(dy, dx) * (180.0f / PI); 
  DrawTexturePro(player.playerTex,(Rectangle){0,0,player.playerTex.width,player.playerTex.height},(Rectangle){player.playerPos.x,player.playerPos.y,player.playerTex.width,player.playerTex.height},(Vector2){(float)player.playerImg.width/2,(float)player.playerImg.height/2},rotation+90,RAYWHITE);
  /*if(IsMouseButtonPressed(0)) DrawCircle(dx + player.playerPos.x, dy + player.playerPos.y, 20 , RED);//draws attackkk*/
  camera.target = player.playerPos; 
  camera.offset = (Vector2){(float)SCREEN_WIDTH/2,(float)SCREEN_HEIGHT/2}; 
  camera.rotation = 0.0f; 
  camera.zoom = 2.0f; 
}
int main() 
{
  srand(time(0));
  player.playerPos = (Vector2){0.0f,0.0f};
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");
  SetTargetFPS(60);
  player.playerImg = LoadImage("res/player.png");
  player.playerTex = LoadTextureFromImage(player.playerImg);
  initSpawner();
  while(!WindowShouldClose())
  {
    if(player.playerTex.width == 0)
    {
      ClearBackground(BLACK);
      DrawText("YOU DIED!!", player.playerPos.x,player.playerPos.y, 40, WHITE);
    }
    enemySpawnTimer += GetFrameTime();
    if(enemySpawnTimer >= ENEMY_SPAWN_TIME)
    {
      spawnEnemy();
      enemySpawnTimer = 0.0f;
    }
    hordeTimer += 2;
    if(hordeTimer >= 6)
    {
      spawnEnemy();
      hordeTimer = 0.0f;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    DrawTexturePro(attack.explosion,attack.frameRec,(Rectangle){0,0,100,100},attack.attackPos,0,WHITE);
    updatePlayer();
    worldBounds();
    input();
    Enemy();
    drawSpawner();
    drawEnemy();
    EndMode2D();
    EndDrawing();
  }
  UnloadImage(player.playerImg);
  UnloadTexture(player.playerTex);
  CloseWindow();
  return 0;
}
