#include "commonFunc.h"
#include "character.h"
#include "map.h"
#include "enemy.h"

Character::Character()
{
    pos_x_ = 90;
    pos_y_ = 418;
    vel_x_ = 0;
    vel_y_ = 0;
    gravity_y = 1000.0;
    gravity_x = 600.0;
    jump = -400.0;
    on_ground_ = false;
    flag_right_ = false;
    flag_left_ = false;
    is_right_ = false;
    player_.SetRect((int)pos_x_, (int)pos_y_);
    hp_ = max_hp_ = 500;
    mp_ = max_mp_ = 200;  
    attack_damage_ = 20;
    attack_range_ = 40;
    last_attack_time_ = 0;
    attack_cooldown_ = 500; 

    quest_stage_ = 0;
    active_quest_ = nullptr;
}

void Character::SetQuest(Quest* q)
{
    active_quest_ = q;
}

void Character::DoQuest()
{
    if(active_quest_)
    {
        active_quest_->InCurrent();
        if(active_quest_->IsCompleted())
        {
            NextQuest();
        }
    }
}

void Character::QuestReward()
{
    int type = active_quest_->GetRewardType();
    if(type == 1) 
    {
        attack_cooldown_ = 300;
        attack_damage_ +=10;
        max_hp_ += 200;
        max_mp_ += 100;
    }
    else if(type == 2) 
    {
        jump = -400;
        max_hp_ += 200;
        max_mp_ += 50;
        attack_damage_ +=20;
    }
    else if(type == 3)
    {
        max_hp_ += 200;
        max_mp_ += 50;
        attack_damage_ +=20;
    }
}

void Character::NextQuest()
{
    quest_stage_ ++;
    active_quest_ = nullptr;
}

std::string Character::GetCurrentQuestInfo()
{
    if(active_quest_)
    {
        return "Mission: " + active_quest_->GetDescription() + 
        " (" + std::to_string(active_quest_->GetProgress()) +
        "/" + std::to_string(active_quest_->GetGoal()) + ")";
    }
    return "!Mission";
}

void Character::TakeDamage(int damage)
{
    hp_ -= damage;
    if (hp_ < 0) hp_ = 0;
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
            if (map_data[i][j] == 0) 
            {
                continue;
            }
            if (map_data[i][j] == 2)
            {
                if (vel_y_ > 0)
                {
                    int platform_y = i * TILE_SIZE_HEIGHT;
                    if (pos_y_ + h_player <= platform_y + 5)
                    {
                        return true;
                    }
                }
                continue; 
            }
            if(map_data[i][j] == 1) return true;

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

SDL_Rect Character::GetAttackBox()
{
    SDL_Rect attack_rect;

    if (is_right_) 
    {
        attack_rect = { int(pos_x_) , int (pos_y_), attack_range_ + 21, 38 };
    } 
    else 
    {
        attack_rect = { int (pos_x_) - attack_range_, int (pos_y_), attack_range_ + 21, 38 };
    }

    return attack_rect;
}

void Character::Attack(std::vector<Enemy>& enemies, Character& player)
{
    Uint32 now = SDL_GetTicks();
    if (now - last_attack_time_ < attack_cooldown_) return; 

    is_attacking_ = true;
    last_attack_time_ = now;

    SDL_Rect attack_box = GetAttackBox();

    for (Enemy& enemy : enemies)
    {
        if (!enemy.IsDead())
        {
            SDL_Rect enemy_box = enemy.GetHitBox();
            if (SDL_HasIntersection(&attack_box, &enemy_box))
            {
                if(mp_>=5)
                {
                    enemy.TakeDamage(attack_damage_, player);
                    mp_-=5;
                }
            }
        }
    }
}

void Character::HandleInput(SDL_Event& event, std::vector<Enemy>& enemies, Character& player)
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
                            is_right_ = false;
                            break;
            case SDLK_RIGHT: target_speed_x_ = SPEED;
                            vel_x_= target_speed_x_;
                            flag_right_ = true;
                            is_right_ = true;
                            break;
            case SDLK_SPACE: Attack(enemies, player);
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: flag_left_ = false;
                            if(!flag_right_) target_speed_x_ = 0;
                            else 
                            {
                                target_speed_x_ = SPEED;
                                vel_x_ = target_speed_x_;
                            }
                            break;

            case SDLK_RIGHT: flag_right_ = false;
                            if(!flag_left_) target_speed_x_ = 0;
                            else 
                            {
                                target_speed_x_ = -SPEED;
                                vel_x_ = target_speed_x_;
                            }
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
    std::string text = "X: " + std::to_string((int)pos_x_) + "  Y: " + std::to_string((int)pos_y_) + "  HP: "+ std::to_string(hp_) + "  MP: " + std::to_string(mp_);

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
