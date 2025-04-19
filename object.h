#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "defs.h"
#include "func.h"

struct GameState{
    static constexpr int MENU = 0;
    static constexpr int PLAYING = 1;
    static constexpr int PAUSED = 2;
    static constexpr int GAMEOVER = 3;
    int currentState;

    GameState();
    void ChangeState(int newState);

};

struct Background{
    int layers;
    vector<string> path_str;
    vector<SDL_Texture*> background_layers;
    vector<float> speed_layers;
    vector<SDL_Rect> bgxRect1;
    vector<SDL_Rect> bgxRect2;

    Background(SDL_Renderer* renderer, int layers_);
    void Update(SDL_Renderer* renderer, float deltaTime);
    void Render(SDL_Renderer* renderer);
    void Reset();
    void Quit();
};

struct Enemy {
    int x, y;
    int w, h;
    int y_start;
    int sizegap = 3;
    float speed;
    char* path_tex_explose = "Graphic/Explose/explosion.png";
    SDL_Texture* tex;
    SDL_Texture* tex_explose;
    bool isExplose = false;
    int frame = 0;
    int frame_explose = 0;
    int max_frames = 2;
    int max_frames_explose = 5;
    float frame_counter = 0;
    float frame_counter_explose = 0;

    Enemy(SDL_Renderer* renderer, int x_, int y_, int w_, int h_, float speed_, string name);
    void ResetPosition();
    void Update(SDL_Renderer* renderer, float deltaTime);
    void Render(SDL_Renderer* renderer);
    void Explose();
    void UpdateExplose(SDL_Renderer* renderer, float deltaTime);
    void Quit();
};

struct Bullet{
    int x, y;
    int w, h;
    int sizegap = 2;
    SDL_Texture* tex[2];
    bool isSpawn = false;
    int frame = 0;
    int max_frames = 2;
    int frame_counter = 0;

    Bullet(SDL_Renderer* renderer, int w_, int h_, string name);
    void Shoot(int pos_x, int pos_y);
    void Update(SDL_Renderer* renderer, float deltaTime);
    void Render(SDL_Renderer* renderer);
    bool Check(int x_, int y_, int w_, int h_);
    void End();
    void Quit();

};

struct Score{
    int x, y;
    int w, h;
    int size = 24;
    int point = 0;
    string font_name;
    SDL_Color color;
    SDL_Rect rect = {0, 0, 0, 0};
    SDL_Texture* tex;

    Score(SDL_Renderer* renderer, string font_name_, SDL_Color color_);
    void Update(SDL_Renderer* renderer);
    void Quit();
};

struct Player{
    int x, y;
    int w = 16;
    int h = 24;
    int sizegap = 3;
    float speed;
    char* path = "Graphic/Player/player.png";
    SDL_Texture* tex;
    bool key[SDL_NUM_SCANCODES] = {false};
    bool isSpawn;
    bool isLeft = false;
    bool isRight = false;
    int frame = 0;
    int max_frames = 2;
    float frame_counter = 0;
    Uint32 respawnTimer;

    Player(SDL_Renderer* renderer);
    void handleInput(SDL_Event& event);
    void Update(SDL_Renderer* renderer, float deltaTime);
    bool Check(int x_, int y_, int w_, int h_);
    void Render(SDL_Renderer* renderer);
    void Reset();
    void ResetState();
    void Quit();
};
#endif // OBJECT_H_INCLUDED
