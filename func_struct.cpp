#include "func_struct.h"

void UpdateBulletAndEnemy(vector<Bullet>& bullet1, vector<Enemy>& enemy1, Score& score1, Mix_Chunk* sound) {
    if (bullet1.empty()) return;
    size_t i = 0;
    while (i < bullet1.size()) {
        size_t j = 0;
        bool isDelete = false;
        while (j < enemy1.size()) {
            if (bullet1[i].Check(enemy1[j].x, enemy1[j].y, enemy1[j].w, enemy1[j].h)) {
                Mix_PlayChannel(-1, sound, 0);
                bullet1.erase(bullet1.begin() + i);
                enemy1[j].Explose();
                score1.point++;
                isDelete = true;
                break;
            } else ++j;
        }
        if (!isDelete) {
            if (!bullet1[i].isSpawn) {
                bullet1.erase(bullet1.begin() + i);
            } else ++i;
        }
    }
}

void UpdatePlayerAndEnemy(Player& player1, vector<Enemy>& enemy1, int& dead, Mix_Chunk* sound1, Mix_Chunk* sound2, bool& over){
    for (auto& e : enemy1){
        if (player1.Check(e.x, e.y, e.w, e.h)){
            Mix_PlayChannel(-1, sound1, 0);
            Mix_PlayChannel(-1, sound2, 0);
            dead++;
            player1.Reset();
            e.Explose();
            if (dead == 4) over = true;
            break;
        }
    }
}

void BornEnemy(SDL_Renderer* renderer, vector<Enemy>& enemy1, int size) {
    int size_pixel[3][3] = {{16, 16, 3}, {32, 16, 3}, {32, 32, 3}};
    float speed[3] = {5.0f, 4.0f, 3.0f};
    string name[3] = {"enemy_small", "enemy_medium", "enemy_big"};
    int current_size = enemy1.size();  // Số enemy hiện tại
    int enemies_to_add = size - current_size;  // Số enemy cần thêm
    const int max_attempts = 1000;
    for (int i = 0; i < enemies_to_add; i++) {
        int type = rand() % 3;
        int y = size_pixel[type][1] * size_pixel[type][2] +
                rand() % ((SCREEN_HEIGHT / 2) - size_pixel[type][1] * size_pixel[type][2] + 1);
        int x;
        bool valid_position = false;
        for (int attempt = 0; attempt < max_attempts; attempt++) {
            x = rand() % (SCREEN_WIDTH - size_pixel[type][0] * size_pixel[type][2]);
            valid_position = true;
            for (const auto& e_ : enemy1) {
                if (abs(x - e_.x) <= 20) {
                    valid_position = false;
                    break;
                }
            }
            if (valid_position) break;
        }
        Enemy e(renderer, x, -y, size_pixel[type][0], size_pixel[type][1], speed[type], name[type]);
        enemy1.push_back(e);
    }
}

void PauseAll(SDL_Renderer* renderer, Background& background1, Player& player1, vector<Enemy>& enemy1, vector<Bullet> bullet1){
    background1.Render(renderer);
    player1.Render(renderer);
    for (auto& bullet : bullet1) {
        bullet.Render(renderer);
    }
    for (auto& e : enemy1) {
        e.Render(renderer);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);
}

