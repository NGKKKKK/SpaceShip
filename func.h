#ifndef FUNC_H_INCLUDED
#define FUNC_H_INCLUDED

#include "defs.h"

void Init(SDL_Window*& window, SDL_Renderer*& renderer); //Tạo window + renderer
void Quit(SDL_Window*& window, SDL_Renderer*& renderer); //Giải phóng tài nguyên + thoát
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path); //Load ảnh -> texture
SDL_Texture* LoadTextureFont(SDL_Renderer* renderer, string font_name, string text, int size, const SDL_Color& color, SDL_Rect& rect); //Load texture chữ có dùng font
bool IsMouseInside(int mouseX, int mouseY, const SDL_Rect& buttonRect); //Kiểm tra xem có trỏ vào nút ko ???


#endif // FUNC_H_INCLUDED
