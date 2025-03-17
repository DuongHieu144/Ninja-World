#include "commonFunc.h"
#include "character.h"
#include "map.h"

Character::Character()
{
    pos_x_ = 100;
    pos_y_ = 100;
    vel_x_ = 0;
    vel_y_ = 0;
    gravity_ = 1000.0;
    max_fall_ = 500.0;
    jump = -450.0;
    on_ground_ = false;
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
            case SDLK_LEFT: vel_x_ = -SPEED; break;
            case SDLK_RIGHT: vel_x_ = SPEED; break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
            case SDLK_RIGHT:
                vel_x_ = 0;
                break;
        }
    }
}

bool Character::CheckCollision(int x, int y, std::vector<std::vector<int> >& map_data)
{
    int left_tile = x / TILE_SIZE_WIDTH;
    int right_tile = (x + TILE_SIZE_WIDTH - 1) / TILE_SIZE_WIDTH;
    int top_tile = y / TILE_SIZE_HEIGHT;
    int bottom_tile = (y + TILE_SIZE_HEIGHT - 1) / TILE_SIZE_HEIGHT;

    for (int i = top_tile; i <= bottom_tile; ++i) {
        for (int j = left_tile; j <= right_tile; ++j) 
        {
            if (map_data[i][j] != 0) 
            {
                return true;
            }
        }
    }
    if (x < 0 || x + TILE_SIZE_WIDTH > map_width || y < 0 || y + TILE_SIZE_HEIGHT > map_height)
    {
        if (y + TILE_SIZE_HEIGHT > map_height)
            on_ground_ = true;
        return true;
    }

    return false;
}



void Character::Move(double delta_time, std::vector<std::vector<int> >& map_data)
{
    double new_pos_x = pos_x_ + vel_x_ * delta_time;
    if (!CheckCollision((int)new_pos_x, (int)pos_y_, map_data))
        pos_x_ = new_pos_x;

    vel_y_ += gravity_ * delta_time;
    if (vel_y_ > max_fall_) vel_y_ = max_fall_;

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

    player_.SetRect((int)pos_x_, (int)pos_y_);
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
