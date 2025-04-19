#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <iostream>  // Chỉ include thư viện cần thiết
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr char TITLE[] = "GAME";
constexpr int NUMS_LAYER = 2;
constexpr Uint32 FRAME_DELAY = 16;
constexpr float FRAME_DELAY_BULLET = 0.1f;
constexpr float FRAME_DELAY_EXPLOSE = 3.0f;
constexpr Uint32 BULLET_COOLDOWN = 200;
constexpr Uint32 RESPAWN_TIME = 1000;

#endif // DEFS_H_INCLUDED
