#include "func.h"

void Init(SDL_Window*& window, SDL_Renderer*& renderer){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0){
        cerr << "Lỗi khởi tạo SDL: " << SDL_GetError() << endl;
        return;
    }
    if (TTF_Init() < 0){
        cerr << "Lỗi khởi tạo SDL_Ttf" << TTF_GetError() << endl;
        return;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0){
        cerr << "Lỗi khởi tạo SDL_Image: " << IMG_GetError() << endl;
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Lỗi khởi tạo SDL_Mixer: " << Mix_GetError() << endl;
        return;
    }
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window){
        cerr << "Lỗi khởi tạo SDL_Window: " << SDL_GetError() << endl;
        IMG_Quit();
        SDL_Quit();
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        cerr << "Lỗi khởi tạo SDL_Renderer :" << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        window = nullptr;
        IMG_Quit();
        SDL_Quit();
    }
}

void Quit(SDL_Window*& window, SDL_Renderer*& renderer){
    if (renderer){
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window){
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path){
    if (!renderer){
        cerr << "Lỗi renderer" << endl;
        return nullptr;
    }
    SDL_Surface* sur = IMG_Load(path);
    if (!sur){
        cerr << "Lỗi load ảnh: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sur);
    SDL_FreeSurface(sur);
    if (!tex) cerr << "Lỗi tạo texture từ surface: " << SDL_GetError() << endl;
    return tex;
}

SDL_Texture* LoadTextureFont(SDL_Renderer* renderer, string font_name, string text, int size, const SDL_Color& color, SDL_Rect& rect) {
    string path_str = "Font/" + font_name + ".otf";
    TTF_Font* font = TTF_OpenFont(path_str.c_str(), size);
    if (!font) {
        cerr << "Lỗi load font: " << TTF_GetError() << endl;
        return nullptr;
    }
    SDL_Surface* sur = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!sur) {
        cerr << "Lỗi tạo ảnh từ chữ: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        return nullptr;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sur);
    rect.w = sur -> w;
    rect.h = sur -> h;
    rect.x = (SCREEN_WIDTH - rect.w)/2;
    rect.y = (SCREEN_HEIGHT - rect.h)/2;
    SDL_FreeSurface(sur);
    if (!tex) {
        cerr << "Lỗi tạo texture từ surface: " << SDL_GetError() << endl;
    }
    TTF_CloseFont(font);
    return tex;
}

bool IsMouseInside(int mouseX, int mouseY, const SDL_Rect& buttonRect){
    return (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
            mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h);
}


