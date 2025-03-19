#include "commonFunc.h"
#include "character.h"
#include "map.h"

Character::Character()
{
    pos_x_ = 100;
    pos_y_ = 100;
    vel_x_ = 0;
    vel_y_ = 0;
    gravity_y = 1000.0;
    gravity_x = 600.0;
    jump = -350.0;
    on_ground_ = false;
    flag_right_ = false;
    flag_left_ = false;
    player_.SetRect((int)pos_x_, (int)pos_y_);
}

Character::~Character()
{
    player_.Free();
}

bool Character::LoadImg(std::string path, SDL_Renderer* screen)
{
    return player_.LoadImg(path, screen);
}

bool Character::CheckCollision(int x, int y, std::vector<std::vector<int> >& map_data)
{
    int left_tile = x / TILE_SIZE_WIDTH;
    int right_tile = (x + w_player - 1) / TILE_SIZE_WIDTH;
    int top_tile = y / TILE_SIZE_HEIGHT;
    int bottom_tile = (y + h_player - 1) / TILE_SIZE_HEIGHT;

    for (int i = top_tile; i <= bottom_tile; ++i) 
    {
        for (int j = left_tile; j <= right_tile; ++j) 
        {
            if (map_data[i][j] != 0) 
            {
                return true;
            }
        }
    }
    if (x < 0 || x + w_player > map_width || y < 0 || y + h_player > map_height)
    {
        if (y + h_player > map_height)
            on_ground_ = true;
        return true;
    }

    return false;
}

void Character::HandleInput(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                if (on_ground_)
                {
                    vel_y_ = jump;
                    on_ground_ = false;
                }
                break;
            case SDLK_LEFT: target_speed_x_ = -SPEED;
                            vel_x_= target_speed_x_; 
                            flag_left_ = true;
                            break;
            case SDLK_RIGHT: target_speed_x_ = SPEED;
                            vel_x_= target_speed_x_;
                            flag_right_ = true;
                            break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: flag_left_ = false;
                            if(!flag_right_) target_speed_x_ = 0;
                            break;

            case SDLK_RIGHT: flag_right_ = false;
                            if(!flag_left_) target_speed_x_ = 0;
                            break;
        }
    }
}

void Character::Move(double delta_time, std::vector<std::vector<int> >& map_data)
{
    bool was_on_ground = on_ground_;
    if(on_ground_) 
    {
        if (vel_x_ < target_speed_x_)
        {
            vel_x_ += gravity_x * delta_time;
            if(vel_x_ > target_speed_x_) vel_x_ = target_speed_x_;
        }
        else if (vel_x_ > target_speed_x_)
        {
            vel_x_ -= gravity_x * delta_time;
            if(vel_x_ < target_speed_x_) vel_x_ = target_speed_x_;
        }
    }
    double new_pos_x = pos_x_ + vel_x_ * delta_time;
    if (!CheckCollision((int)new_pos_x, (int)pos_y_, map_data))
    {
        pos_x_ = new_pos_x;
    }
    else
    {
        if(vel_x_ > 0)
        {
            vel_x_ = 0;
        }
    }

    vel_y_ += gravity_y * delta_time;
    double new_pos_y = pos_y_ + vel_y_ * delta_time;
    if (!CheckCollision((int)pos_x_, (int)new_pos_y, map_data))
    {
        pos_y_ = new_pos_y;
        on_ground_ = false;
    }
    else
    {
        if (vel_y_ > 0)
        {
            vel_y_ = 0;
            on_ground_ = true;
        }
    }
    if (!was_on_ground && on_ground_) 
    {
        if(!flag_left_ && !flag_right_)
        {
            vel_x_ = 0;           
            target_speed_x_ = 0; 
        }   
        else
        {
            if(flag_left_) target_speed_x_ = -SPEED;
            else target_speed_x_ = SPEED;
        }
    }
    
    player_.SetRect((int)pos_x_, (int)pos_y_);
}

void Character::ShowPosition(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* camera) 
{
    std::string text = "X: " + std::to_string((int)pos_x_) + "  Y: " + std::to_string((int)pos_y_);

    SDL_Color textColor = {255, 255, 255}; 

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface != nullptr) 
    {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textW = textSurface->w;
        int textH = textSurface->h;

        SDL_Rect renderQuad = {SCREEN_WIDTH - textW - 10, 10, textW, textH};  

        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}


void Character::Render(SDL_Renderer* des, SDL_Rect* camera)
{
    SDL_Rect render_rect;
    render_rect.x = (int)pos_x_ - camera->x;
    render_rect.y = (int)pos_y_ - camera->y;
    render_rect.w = TILE_SIZE_WIDTH;
    render_rect.h = TILE_SIZE_HEIGHT;

    player_.SetRect(render_rect.x, render_rect.y);
    player_.Render(des, NULL);
}
