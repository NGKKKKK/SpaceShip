//Khai báo
#include "defs.h"
#include "object.h"
#include "func.h"
#include "func_struct.h"

//Xử lí chính
int main(int argc, char*argv[]) {
    Uint32 frameStart;
    int frameTime;
    //Khởi tạo
    srand(time(0));
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Init(window, renderer);

    //Tạo background
    Background background1(renderer, 2);

    //Tạo tiêu đề game
    SDL_Texture* gametitle = LoadTexture(renderer, "Graphic/Background/Gametitle.png");
    SDL_Rect gametitle_rect = {(SCREEN_WIDTH - 504)/2,(SCREEN_HEIGHT - 154)/2 - 70, 504, 154};
    string playtitle_text = "Click/Press Space to play !!!";
    SDL_Color color_white = {255, 255, 255};
    SDL_Rect playtitle_rect = {0, 0, 0, 0};
    SDL_Texture* playtitle = LoadTextureFont(renderer, "Minecraft", playtitle_text, 20, color_white, playtitle_rect);
    playtitle_rect.y += 50;
    Uint8 alpha = 255;
    bool fadeOut = true;
    Uint32 fadeSpeed = 4;

    //Score
    Score score1(renderer, "Minecraft", color_white);

    //Tạo player
    Player player1(renderer);

    //Tạo enemy
    vector<Enemy> enemy1;

    //Tạo bullet
    vector <Bullet> bullet1;

    //Tạo game state
    GameState mygame;

    //Title over
    SDL_Rect overtitle_rect = {0, 0, 0, 0};
    SDL_Rect re1_rect = {0, 0, 0, 0};
    SDL_Rect re2_rect = {0, 0, 0, 0};
    SDL_Rect pausetitle_rect = {0, 0, 0, 0};
    SDL_Texture* overtitle = LoadTextureFont(renderer, "Minecraft", "Game Over !!!", 60, color_white, overtitle_rect);
    overtitle_rect.y -= 60;
    SDL_Texture* pausetitle = LoadTextureFont(renderer, "Minecraft", "Paused", 60, color_white, pausetitle_rect);
    SDL_Texture* re1 = LoadTextureFont(renderer, "Minecraft", "Press Enter/Space to return to menu", 20, color_white, re1_rect);
    re1_rect.y += 50;

    //Sound effect
    Mix_Chunk* start_sound = Mix_LoadWAV("Sound/start.wav");
    Mix_Chunk* shoot_sound = Mix_LoadWAV("Sound/shoot.wav");
    Mix_Chunk* dead_sound = Mix_LoadWAV("Sound/dead.wav");
    Mix_Chunk* kill_sound = Mix_LoadWAV("Sound/kill.wav");
    Mix_Chunk* return_sound = Mix_LoadWAV("Sound/return.wav");
    Mix_Chunk* gameover_sound = Mix_LoadWAV("Sound/gameover.wav");
    Mix_Chunk* pause_sound = Mix_LoadWAV("Sound/pause.wav");
    Mix_Chunk* unpause_sound = Mix_LoadWAV("Sound/unpause.wav");
    Mix_Chunk* explosion_sound = Mix_LoadWAV("Sound/explosion2.wav");
    Mix_VolumeChunk(shoot_sound, MIX_MAX_VOLUME / 10);
    Mix_VolumeChunk(dead_sound, MIX_MAX_VOLUME / 5);
    Mix_VolumeChunk(return_sound, MIX_MAX_VOLUME / 2);
    Mix_VolumeChunk(explosion_sound, MIX_MAX_VOLUME / 5);

    //Theme song
    Mix_Music* bgMusic = Mix_LoadMUS("Sound/background.mp3");
    Mix_VolumeMusic(0);
    Mix_PlayMusic(bgMusic, -1);

    //Loop game
    bool running = true;
    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 lastBulletTime = SDL_GetTicks();
    int musicVolume = 0;
    Uint32 musicStartTime = SDL_GetTicks();
    int level = 0;
    bool over = false;
    vector<int> levelUp(20);
    levelUp[0] = 50;
    for (int i = 1; i < 20; i++){
        levelUp[i] = levelUp[i-1] + 50;
    }
    int dead = 0;
    while (running){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 16.0f;
        lastTime = currentTime;
        if (currentTime - musicStartTime >= 100 && musicVolume < MIX_MAX_VOLUME) {
            musicVolume += 5;
            Mix_VolumeMusic(musicVolume);
            musicStartTime = currentTime;
        }
        int pos_x, pos_y;
        player1.handleInput(event);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            switch (mygame.currentState) {
                case GameState::MENU:
                    SDL_GetMouseState(&pos_x, &pos_y);
                    if ((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT &&
                         !IsMouseInside(pos_x, pos_y, gametitle_rect)) ||
                        event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                            Mix_PlayChannel(-1, start_sound, 0);
                            player1.ResetState();
                            level = 0;
                            dead = 0;
                            over = false;
                            if (!enemy1.empty()) enemy1.clear();
                            BornEnemy(renderer, enemy1, 6);
                            score1.point = 0;
                            mygame.ChangeState(GameState::PLAYING);
                    }
                    break;

                case GameState::PLAYING:
                    player1.handleInput(event);
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE){
                        Mix_PlayChannel(-1, pause_sound, 0);
                        mygame.ChangeState(GameState::PAUSED);
                    }
                    break;

                case GameState::PAUSED:
                    if (event.type == SDL_KEYDOWN){
                        if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE){
                            Mix_PlayChannel(-1, return_sound, 0);
                            mygame.ChangeState(GameState::MENU);
                        }
                        else if (event.key.keysym.sym == SDLK_ESCAPE){
                            Mix_PlayChannel(-1, unpause_sound, 0);
                            mygame.ChangeState(GameState::PLAYING);
                        }
                    }
                    break;

                case GameState::GAMEOVER:
                    enemy1.clear();
                    if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_RETURN)
                        || event.key.keysym.sym == SDLK_SPACE){
                        Mix_PlayChannel(-1, return_sound, 0);
                        mygame.ChangeState(GameState::MENU);
                    }
                    break;
            }
        }
        if (player1.isSpawn && player1.key[SDL_SCANCODE_SPACE] && currentTime - lastBulletTime >= BULLET_COOLDOWN) {
                bullet1.push_back(Bullet(renderer, 5, 13, "bullet"  ));
                bullet1.back().Shoot(((2*player1.x + player1.w*player1.sizegap - 5*3)/2), player1.y);
                Mix_PlayChannel(-1, shoot_sound, 0);
                lastBulletTime = currentTime;
        }
        if (level < int(levelUp.size()) && score1.point == levelUp[level]) {
            BornEnemy(renderer, enemy1, level + 1 + 6);
            level++;
            for (auto& e : enemy1) {
                e.speed += 0.5f;
            }
        }
        if (fadeOut) {
        alpha -= fadeSpeed;
        if (alpha <= 50) fadeOut = false;
        } else {
        alpha += fadeSpeed;
        if (alpha >= 255) fadeOut = true;
        }
        SDL_SetTextureAlphaMod(playtitle, alpha);
        SDL_SetTextureAlphaMod(re1, alpha);
        //Nền
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 225);
        SDL_RenderClear(renderer);
        switch (mygame.currentState){
            //MENU
            case GameState::MENU:
                background1.Update(renderer, deltaTime);
                SDL_RenderCopy(renderer, gametitle, NULL, &gametitle_rect);
                SDL_RenderCopy(renderer, playtitle, NULL, &playtitle_rect);
                break;

            //PLAY
            case GameState::PLAYING:
                background1.Update(renderer, deltaTime);
                UpdateBulletAndEnemy(bullet1, enemy1, score1, explosion_sound);
                UpdatePlayerAndEnemy(player1, enemy1, dead, explosion_sound, dead_sound, over);
                player1.Update(renderer, deltaTime);
                for (auto& bullet : bullet1) {
                    bullet.Update(renderer, deltaTime);
                }
                player1.Update(renderer, deltaTime);
                for (auto& e : enemy1) {
                    e.Update(renderer, deltaTime);
                }
                if (over){
                    mygame.ChangeState(GameState::GAMEOVER);
                    Mix_PlayChannel(-1, gameover_sound, 0);
                }
                score1.Update(renderer);
                break;

            //PAUSE
            case GameState::PAUSED:
                PauseAll(renderer, background1, player1, enemy1, bullet1);
                SDL_RenderCopy(renderer, pausetitle, NULL, &pausetitle_rect);
                SDL_RenderCopy(renderer, re1, NULL, &re1_rect);
                break;

            //OVER
            case GameState::GAMEOVER:
                PauseAll(renderer, background1, player1, enemy1, bullet1);
                SDL_Texture* re2 = LoadTextureFont(renderer, "Minecraft", "Your Score: " + to_string(score1.point), 35, color_white, re2_rect);
                SDL_RenderCopy(renderer, overtitle, NULL, &overtitle_rect);
                SDL_RenderCopy(renderer, re1, NULL, &re1_rect);
                SDL_RenderCopy(renderer, re2, NULL, &re2_rect);
                break;
        }

        //Hiển thị
        SDL_RenderPresent(renderer);
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < FRAME_DELAY) SDL_Delay(FRAME_DELAY - frameTime);
    }

    //Thoát
    for (auto& e : enemy1){
        e.Quit();
    }
    SDL_DestroyTexture(gametitle);
    SDL_DestroyTexture(playtitle);
    background1.Quit();
    score1.Quit();
    player1.Quit();
    Mix_FreeChunk(start_sound);
    Mix_FreeChunk(shoot_sound);
    Mix_FreeChunk(kill_sound);
    Mix_FreeChunk(return_sound);
    Mix_FreeChunk(pause_sound);
    Mix_FreeChunk(unpause_sound);
    Mix_FreeMusic(bgMusic);
    Mix_FreeChunk(gameover_sound);
    Mix_FreeChunk(explosion_sound);
    Quit(window, renderer);
    return 0;
}
