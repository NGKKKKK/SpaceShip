#ifndef FUNC_STRUCT_H_INCLUDED
#define FUNC_STRUCT_H_INCLUDED

#include "object.h"

void UpdateBulletAndEnemy(vector<Bullet>& bullet1, vector<Enemy>& enemy1, Score& score1, Mix_Chunk* sound);
void UpdatePlayerAndEnemy(Player& player1, vector<Enemy>& enemy1, int& dead, Mix_Chunk* sound1, Mix_Chunk* sound2, bool &over);
void BornEnemy(SDL_Renderer* renderer, vector<Enemy>& enemy1, int size);
void PauseAll(SDL_Renderer* renderer, Background& background1, Player& player1, vector<Enemy>& enemy1, vector<Bullet> bullet1);

#endif // FUNC_STRUCT_H_INCLUDED
