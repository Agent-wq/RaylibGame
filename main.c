#include "main.h"
Camera2D camera; // camera
player p1;
attack a1;
spawners spawnPoint[NUM_SPAWNER];
bounds walls;
void initSpawner() {
  for (int i = 0; i < NUM_SPAWNER; i++) {
    float x = rand() % 10000;
    float y = rand() % 10000;
    spawnPoint[i].spawnerBod.x = x;
    spawnPoint[i].spawnerBod.y = y;
    spawnPoint[i].spawnerBod.width = 90;
    spawnPoint[i].spawnerBod.height = 90;
    spawnPoint[i].spawnerPos = (Vector2){x, y};
    // this is just a debug block
    /*spawnPoint[i].spawnerBod.height = 20;*/
    /*spawnPoint[i].spawnerBod.width = 20;*/
    /*spawnPoint[i].spawnerCol = WHITE;*/
    // upto here
    // make sure spawners are spawned far apart
  }
}
// draw spawners for debug
void drawSpawner() {
  for (int i = 0; i < NUM_SPAWNER; i++) {
    DrawRectangleRec(spawnPoint[i].spawnerBod, WHITE);
    /*DrawLine(spawnPoint[i].spawnerPos.x,spawnPoint[i].spawnerPos.y,player.playerPos.x,player.playerPos.y,WHITE);*/
  }
}
float hordeTimer = 0.0f;
enemies enemy[NUM_ENEMIES];
float enemySpawnTimer = 0.0f;
void spawnEnemy() {
  for (int i = 0; i < NUM_ENEMIES; i++) {
    if (!enemy[i].isActive) {
      int spawnIndex = rand() % NUM_SPAWNER;
      enemy[i].enemyBod = spawnPoint[spawnIndex].spawnerBod;
      enemy[i].isActive = true;
      enemy[i].enemyBod.height = 10;
      enemy[i].enemyBod.width = 10;
      enemy[i].enemyCol = RED;
      return;
    }
  }
}
void worldBounds() {
  walls.downRec = (Rectangle){-100, 1000, 1500, 10};
  walls.upRec = (Rectangle){-100, -1000, 1500, 10};
  walls.leftRec = (Rectangle){-100, -1000, 10, 2000};
  walls.rightRec = (Rectangle){1400, -1000, 10, 2000};
  DrawRectangleRec(walls.downRec, WHITE);
  DrawRectangleRec(walls.upRec, WHITE);
  DrawRectangleRec(walls.leftRec, WHITE);
  DrawRectangleRec(walls.rightRec, WHITE);
  // debug
  DrawLine(p1.playerPos.x, p1.playerPos.y, walls.upRec.x, walls.upRec.y, WHITE);
  DrawLine(p1.playerPos.x, p1.playerPos.y, walls.leftRec.x, walls.leftRec.y,
           WHITE);
  DrawLine(p1.playerPos.x, p1.playerPos.y, walls.downRec.x, walls.downRec.y,
           WHITE);
  DrawLine(p1.playerPos.x, p1.playerPos.y, walls.rightRec.x, walls.rightRec.y,
           WHITE);
  // check CheckCollisionRecs
  if (CheckCollisionRecs(walls.downRec, p1.playerRect)) {
    p1.playerPos.y -= 10;
  }
  if (CheckCollisionRecs(walls.leftRec, p1.playerRect)) {
    p1.playerPos.x += 10;
  }
  if (CheckCollisionRecs(walls.upRec, p1.playerRect)) {
    p1.playerPos.y += 10;
  }
  if (CheckCollisionRecs(walls.rightRec, p1.playerRect)) {
    p1.playerPos.x -= 10;
  }
}
void Enemy() {
  Vector2 attackPos = GetScreenToWorld2D(GetMousePosition(), camera);
  Vector2 attacCen = {attackPos.x - p1.playerPos.x + p1.playerPos.x,
                      attackPos.y - p1.playerPos.y + p1.playerPos.y};
  for (int i = 0; i < NUM_ENEMIES; i++) {
    if (!enemy[i].isActive)
      continue;
    Vector2 direction = {p1.playerPos.x - enemy[i].enemyBod.x,
                         p1.playerPos.y - enemy[i].enemyBod.y};
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
      direction.x /= length;
      direction.y /= length;
    }
    float enemySpeed = 5.0f;
    enemy[i].enemyBod.x += direction.x * enemySpeed;
    enemy[i].enemyBod.y += direction.y * enemySpeed;
    for (int j = 0; j < NUM_ENEMIES; j++) {
      if (i != j && enemy[i].isActive) {
        float dx = enemy[i].enemyBod.x - enemy[j].enemyBod.x;
        float dy = enemy[i].enemyBod.y - enemy[j].enemyBod.y;
        float distance = sqrtf(dx * dx + dy * dy);
        if (distance < 50.0f) {
          enemy[i].enemyBod.x += dx * 0.05f;
          enemy[i].enemyBod.y += dy * 0.05f;
        }
      }
    }
    if (CheckCollisionCircleRec(attacCen, 20, enemy[i].enemyBod)) {
      DrawCircle(attacCen.x, attacCen.y, 20, RED);
      if (IsMouseButtonDown(0)) {
        enemy[i].isActive = false;
        camera.offset =
            (Vector2){(float)SCREEN_WIDTH / 2 + 30, (float)SCREEN_HEIGHT / 2};
      }
    }
    if (IsKeyDown(KEY_Q)) {
      DrawLine(p1.playerPos.x, p1.playerPos.y, enemy[i].enemyBod.x,
               enemy[i].enemyBod.y, RED);
    }
  }
}
void drawEnemy() {
  for (int i = 0; i < NUM_ENEMIES; i++) {
    if (enemy[i].isActive) {
      DrawRectangleRec(enemy[i].enemyBod, enemy[i].enemyCol);
      /*DrawLine(enemy[i].enemyBod.x, enemy[i].enemyBod.y, player.playerPos.x,
       * player.playerPos.y, WHITE); ///DEBUG thing*/
    }
  }
}
void updatePlayer() {
  /*Vector2 screenPos = GetScreenToWorld2D(player.playerPos, camera);*/
  p1.playerRect = (Rectangle){p1.playerPos.x, p1.playerPos.y, 20, 20};
  for (int i = 0; i < NUM_ENEMIES; i++) {
    DrawRectangleLinesEx(p1.playerRect, 2, WHITE);
    if (CheckCollisionRecs(p1.playerRect, enemy[i].enemyBod)) {
      p1.playerTex.height = 0;
      p1.playerTex.width = 0;
      ClearBackground(BLACK);
    }
  }
}
void input() {
  p1.playerVel = (Vector2){500.0f, 500.0f};
  if (IsKeyDown(KEY_D) && p1.playerTex.width != 0)
    p1.playerPos.x += p1.playerVel.x * GetFrameTime();
  if (IsKeyDown(KEY_A) && p1.playerTex.width != 0)
    p1.playerPos.x -= p1.playerVel.x * GetFrameTime();
  if (IsKeyDown(KEY_W) && p1.playerTex.width != 0)
    p1.playerPos.y -= p1.playerVel.y * GetFrameTime();
  if (IsKeyDown(KEY_S) && p1.playerTex.width != 0)
    p1.playerPos.y += p1.playerVel.y * GetFrameTime();
  // debugging
  if (IsKeyPressed(KEY_F))
    ToggleFullscreen();
  if (IsKeyPressed(KEY_F1))
    HideCursor();
  Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
  float dx = mousePos.x - p1.playerPos.x;
  float dy = mousePos.y - p1.playerPos.y;
  float rotation = atan2f(dy, dx) * (180.0f / PI);
  DrawTexturePro(
      p1.playerTex, (Rectangle){0, 0, p1.playerTex.width, p1.playerTex.height},
      (Rectangle){p1.playerPos.x, p1.playerPos.y, p1.playerTex.width,
                  p1.playerTex.height},
      (Vector2){(float)p1.playerImg.width / 2, (float)p1.playerImg.height / 2},
      rotation + 90, RAYWHITE);
  /*if(IsMouseButtonPressed(0)) DrawCircle(dx + player.playerPos.x, dy +
   * player.playerPos.y, 20 , RED);//draws attackkk*/
  camera.target = p1.playerPos;
  camera.offset = (Vector2){(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
  camera.rotation = 0.0f;
  camera.zoom = 2.0f;
}
int main() {
  srand(time(0));
  p1.playerPos = (Vector2){0.0f, 0.0f};
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");
  SetTargetFPS(60);
  p1.playerImg = LoadImage("res/player.png");
  p1.playerTex = LoadTextureFromImage(p1.playerImg);
  initSpawner();
  while (!WindowShouldClose()) {
    if (p1.playerTex.width == 0) {
      ClearBackground(BLACK);
      DrawText("YOU DIED!!", p1.playerPos.x, p1.playerPos.y, 40, WHITE);
    }
    enemySpawnTimer += GetFrameTime();
    if (enemySpawnTimer >= ENEMY_SPAWN_TIME) {
      spawnEnemy();
      enemySpawnTimer = 0.0f;
    }
    hordeTimer += 2;
    if (hordeTimer >= 6) {
      spawnEnemy();
      hordeTimer = 0.0f;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    DrawTexturePro(a1.explosion, a1.frameRec, (Rectangle){0, 0, 100, 100},
                   a1.attackPos, 0, WHITE);
    updatePlayer();
    worldBounds();
    input();
    Enemy();
    drawSpawner();
    drawEnemy();
    EndMode2D();
    EndDrawing();
  }
  UnloadImage(p1.playerImg);
  UnloadTexture(p1.playerTex);
  CloseWindow();
  return 0;
}
