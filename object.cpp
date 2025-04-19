#include "object.h"

/* --- GameState --- */
GameState::GameState(){
    currentState = MENU;
}
void GameState::ChangeState(int newState){
    currentState = newState;
}
/* --- GameState --- */


/* --- Background --- */
Background::Background(SDL_Renderer* renderer, int layers_){
    if (layers_ <= 0){
        cerr << "Lỗi: số layers phải lớn hơn 0" << endl;
        exit(1);
    }
    layers = layers_;
    path_str.resize(layers);
    background_layers.resize(layers);
    speed_layers.resize(layers);
    bgxRect1.resize(layers);
    bgxRect2.resize(layers);
    int speed_ = 1;
    for (int i = 0; i < layers; i++, speed_++) {
        path_str[i] = "Graphic/Background/Back" + to_string(i + 1) + ".png";

        SDL_Texture* texture = LoadTexture(renderer, path_str[i].c_str());
        if (!texture) {
            cerr << "Lỗi tải texture: " << SDL_GetError() << endl;
            exit(1);
        }
        background_layers[i] = texture;
        speed_layers[i] = speed_;

        bgxRect1[i] = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        bgxRect2[i] = {0, -SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};
    }
}

void Background::Render(SDL_Renderer* renderer){
    for (int i = 0; i < layers; i++){
        SDL_RenderCopy(renderer, background_layers[i], NULL, &bgxRect1[i]);
        SDL_RenderCopy(renderer, background_layers[i], NULL, &bgxRect2[i]);
    }
}

void Background::Update(SDL_Renderer* renderer, float deltaTime){
    for (int i = 0; i < layers; i++){
        SDL_RenderCopy(renderer, background_layers[i], NULL, &bgxRect1[i]);
        SDL_RenderCopy(renderer, background_layers[i], NULL, &bgxRect2[i]);

        float moveStep = speed_layers[i]*deltaTime;
        bgxRect1[i].y += static_cast<int>(moveStep);
        bgxRect2[i].y += static_cast<int>(moveStep);

        if (bgxRect1[i].y >= SCREEN_HEIGHT) bgxRect1[i].y = - SCREEN_HEIGHT;
        if (bgxRect2[i].y >= SCREEN_HEIGHT) bgxRect2[i].y = - SCREEN_HEIGHT;
    }
}

void Background::Reset(){
    for (int i = 0; i < layers; i++){
        bgxRect1[i] = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        bgxRect2[i] = {0, -SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};
    }
}

void Background::Quit(){
    for (auto& tex : background_layers){
        if (tex){
            SDL_DestroyTexture(tex);
            tex = nullptr;
        }
    }
    background_layers.clear();
    path_str.clear();
    speed_layers.clear();
    bgxRect1.clear();
    bgxRect2.clear();
}
/* --- Background --- */

/* --- Enemy ---*/
Enemy::Enemy(SDL_Renderer* renderer, int x_, int y_, int w_, int h_, float speed_, string name){
    x = x_;
    y = y_;
    y_start = y_;
    w = sizegap*w_;
    h = sizegap*h_;
    speed = speed_;
    string path_str = "Graphic/Enemy/" + name + ".png";
    tex = LoadTexture(renderer, path_str.c_str());
    tex_explose = LoadTexture(renderer, path_tex_explose);
}

void Enemy::ResetPosition(){
    int x_tmp;
    do{
        x_tmp = rand() % (SCREEN_WIDTH - w);
    } while(abs(x_tmp - x) <= 10);
    x = x_tmp; // Random vị trí ngang
    y = y_start; // Đặt lại ở phía trên
}

void Enemy::Update(SDL_Renderer* renderer, float deltaTime){
    if (isExplose){
        UpdateExplose(renderer, deltaTime);
        return;
    }
    y += static_cast<int>(speed*deltaTime);
    if (y >= SCREEN_HEIGHT) ResetPosition();
    frame_counter += deltaTime;
    if (frame_counter >= FRAME_DELAY) {
        frame = (frame + 1) % max_frames;
        frame_counter = 0;
    }
    SDL_Rect srcRect = { frame * (w/sizegap), 0, (w/sizegap), (h/sizegap)};
    SDL_Rect dstRect = { x, y, w, h};
    SDL_RenderCopy(renderer, tex, &srcRect, &dstRect);
}

void Enemy::Render(SDL_Renderer* renderer){
    SDL_Rect srcRect = { frame * (w/sizegap), 0, (w/sizegap), (h/sizegap)};
    SDL_Rect dstRect = { x, y, w, h};
    SDL_RenderCopy(renderer, tex, &srcRect, &dstRect);
}

void Enemy::Explose(){
    if (!isExplose){
        isExplose = true;
        frame_explose = 0;
        frame_counter_explose = 0;
    }
}

void Enemy::UpdateExplose(SDL_Renderer* renderer, float deltaTime){
    if (!isExplose) return;
    frame_counter_explose += deltaTime;
    if (frame_counter_explose >= FRAME_DELAY_EXPLOSE){
        frame_explose++;
        frame_counter_explose = 0;
        //Kết thúc nổ
        if (frame_explose >= max_frames_explose){
            ResetPosition();
            isExplose = false;
            frame_explose = 0;
            return;
        }
    }
    SDL_Rect srcRect_explose = { frame_explose * 16, 0, 16, 16}; // Cắt frame trong texture
    SDL_Rect dstRect_explose = { (2*x + w - 16*4)/2, y + h - 16*4, 16*4, 16*4};
    SDL_RenderCopy(renderer, tex_explose, &srcRect_explose, &dstRect_explose);
}

void Enemy::Quit(){
    if (tex){
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
    if (tex_explose){
        SDL_DestroyTexture(tex_explose);
        tex_explose = nullptr;
    }
}
/* --- Enemy ---*/

/* --- Bullet --- */
Bullet::Bullet(SDL_Renderer* renderer, int w_, int h_, string name){
     w = w_*sizegap;
     h = h_*sizegap;
     x = 0;
     y = SCREEN_HEIGHT;
     string path_str1 = "Graphic/Bullet/" + name + "1.png";
     string path_str2 = "Graphic/Bullet/" + name + "2.png";
     tex[0] = LoadTexture(renderer, path_str1.c_str());
     tex[1] = LoadTexture(renderer, path_str2.c_str());
}

void Bullet::Shoot(int pos_x, int pos_y){
    if (isSpawn) return;
    x = pos_x;
    y = pos_y;
    isSpawn = true;
}

void Bullet::Update(SDL_Renderer* renderer, float deltaTime){
    if (!isSpawn) return;
    y -= static_cast<int>(20*deltaTime);
    if (y + h <= 0){
        End();
        return;
    }
    frame_counter += deltaTime;
    if (frame_counter >= FRAME_DELAY_BULLET){
        frame = (frame + 1) % max_frames;
        frame_counter = 0;
    }
    SDL_Rect srcRect = {0, 0, (w/sizegap), (h/sizegap)};
    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(renderer, tex[frame], &srcRect, &dstRect);
}

void Bullet::Render(SDL_Renderer* renderer){
    SDL_Rect srcRect = {0, 0, (w/sizegap), (h/sizegap)};
    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(renderer, tex[frame], &srcRect, &dstRect);
}

bool Bullet::Check(int x_, int y_, int w_, int h_){
    SDL_Rect target = {x_, y_, w_, h_};
    SDL_Rect rect = {x, y, w, h};
    return SDL_HasIntersection(&rect, &target) == SDL_TRUE;
}

void Bullet::End(){
    isSpawn = false;
}

void Bullet::Quit(){
    for (int i = 0; i < max_frames; i++){
        if (tex[i]){
            SDL_DestroyTexture(tex[i]);
            tex[i] = nullptr;
        }
    }
}
/* --- Bullet --- */

/* --- Score --- */
Score::Score(SDL_Renderer* renderer, string font_name_, SDL_Color color_){
    font_name = font_name_;
    color = color_;
    tex = LoadTextureFont(renderer, font_name, "", size, color, rect);
}

void Score::Update(SDL_Renderer* renderer){
    string text = "SCORE : " + to_string(point);
    if (tex) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
    tex = LoadTextureFont(renderer, font_name, text, size, color, rect);
    SDL_RenderCopy(renderer, tex, NULL, &rect);
}

void Score::Quit(){
    if (tex) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
}
/* --- Score --- */

/* --- Player --- */
Player::Player(SDL_Renderer* renderer){
    x = (SCREEN_WIDTH - w*sizegap)/2;
    y = (SCREEN_HEIGHT - h*sizegap) - 50;
    isSpawn = true;
    tex = LoadTexture(renderer, path);
}

void Player::handleInput(SDL_Event& event){
    if (event.type == SDL_KEYDOWN){
        key[event.key.keysym.scancode] = true;
    }
    if (event.type == SDL_KEYUP){
        key[event.key.keysym.scancode] = false;
    }
}

void Player::Update(SDL_Renderer* renderer, float deltaTime){
    frame_counter += deltaTime;
    if (frame_counter >= FRAME_DELAY) {
        frame = (frame + 1) % max_frames;
        frame_counter = 0;
    }
    if (isSpawn){
        SDL_SetTextureAlphaMod(tex, 255);
        float moveStep = 8.0f*deltaTime;
        if (key[SDL_SCANCODE_W]){
            y -= static_cast<int>(moveStep);
            if (y <= 0) y = 0;
        }
        if (key[SDL_SCANCODE_S]){
            y += static_cast<int>(moveStep);
            if (y + h*sizegap >= SCREEN_HEIGHT) y = SCREEN_HEIGHT - h*sizegap;
        }
        if (key[SDL_SCANCODE_A]){
            isLeft = true;
            x -= static_cast<int>(moveStep);
            if (x <= 0) x = 0;
        } else isLeft = false;
        if (key[SDL_SCANCODE_D]){
            isRight = true;
            x += static_cast<int>(moveStep);
            if (x + w*sizegap >= SCREEN_WIDTH) x = SCREEN_WIDTH - w*sizegap;
        } else isRight = false;
        if (key[SDL_SCANCODE_A] && key[SDL_SCANCODE_D]){
            isLeft = false;
            isRight = false;
        }
        int w_ = w*2;
        if (isLeft) w_ = w*1;
        if (isRight) w_ = w*3;
        SDL_Rect srcRect = {w_, h*frame, w, h};
        SDL_Rect dstRect = {x, y, w*sizegap, h*sizegap};
        SDL_RenderCopy(renderer, tex, &srcRect, &dstRect);
    }
    else {
        if (y == SCREEN_HEIGHT - h*sizegap - 50) y = SCREEN_HEIGHT - h*sizegap - 50;
        y --;
        Uint32 elapsed = SDL_GetTicks() - respawnTimer;
        if (elapsed > RESPAWN_TIME) {
            isSpawn = true;
        } else {
            int alpha = min(255, static_cast<int>(255 * elapsed / RESPAWN_TIME));
            if ((elapsed / 200) % 2 == 0) alpha = 0;
            SDL_SetTextureAlphaMod(tex, alpha);
            SDL_Rect srcRect = {w * 2, h * frame, w, h};
            SDL_Rect dstRect = {x, y, w * sizegap, h * sizegap};
            SDL_RenderCopy(renderer, tex, &srcRect, &dstRect);
        }
    }

}

void Player::Render(SDL_Renderer* renderer){
    SDL_Rect srcRect = {w * 2, h * frame, w, h};
    SDL_Rect dstRect = {x, y, w * sizegap, h * sizegap};
    SDL_RenderCopy(renderer, tex, &srcRect, &dstRect);
}

bool Player::Check(int x_, int y_, int w_, int h_){
    if (!isSpawn) return false;
    SDL_Rect target = {x_, y_, w_, h_};
    SDL_Rect rect = {x, y, w*sizegap, h*sizegap};
    return SDL_HasIntersection(&rect, &target) == SDL_TRUE;
}

void Player::Reset(){
    isSpawn = false;
    isRight = false;
    isLeft = false;
    respawnTimer = SDL_GetTicks();
    x = (SCREEN_WIDTH - w * sizegap) / 2;
    y = SCREEN_HEIGHT;
}

void Player::ResetState(){
    isSpawn = true;
    memset(key, false, sizeof(key));
    x = (SCREEN_WIDTH - w * sizegap) / 2;
    y = (SCREEN_HEIGHT - h * sizegap) - 50;
}

void Player::Quit(){
    if (tex){
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }
}

