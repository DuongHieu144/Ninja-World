#include "enemy.h"
#include "character.h"

Enemy::Enemy()
{
    pos_x_ = pos_y_ = 0;
    vel_x_ = 25;
    moving_right_ = true;

    attack_range_ = 40;
    attack_cooldown_ = 2000;
    last_attack_time_ = 0;

    dead_ = false;
    death_time_ = 0;
}

Enemy::~Enemy()
{
    img_enemy_.Free();
}

void Enemy::SetPosition(double x, double y)
{
    pos_x_ = x;
    pos_y_ = y;
}

void Enemy::SetDamage(int damage)
{
    attack_damage_ = damage;
}

void Enemy::SetHP(int hp)
{
    hp_ = max_hp_ = hp;
}

void Enemy::SetPatrolRange(int left, int right)
{
    patrol_left_ = left;
    patrol_right_ = right;
}

void Enemy::TakeDamage(int damage)
{
    if (!dead_)
    {
        hp_ -= damage;
        if (hp_ <= 0)
        {
            hp_ = 0;
            dead_ = true;
            death_time_ = SDL_GetTicks();
        }
    }
}


bool Enemy::IsDead() const { return dead_; }

SDL_Rect Enemy::GetHitBox() const 
{
    return { (int)pos_x_, (int)pos_y_, w_, h_ };
}

bool Enemy::LoadImg(std::string path, SDL_Renderer* screen) 
{
    if (!img_enemy_.LoadImg(path, screen)) return false;
    SDL_Rect rect = img_enemy_.GetRect();
    w_ = rect.w;
    h_ = rect.h;
    return true;
}


void Enemy::RespawnIfNeeded()
{
    if (dead_ && SDL_GetTicks() - death_time_ >= 15000) 
    {
        hp_ = max_hp_;
        dead_ = false;
    }
}

void Enemy::Update(double delta_time, Character& player)
{
    if (dead_) return;
    if(patrol_right_ == patrol_left_){}
    else if (moving_right_)
    {
        pos_x_ += vel_x_ * delta_time;
        if (pos_x_ > patrol_right_)
        {
            pos_x_ = patrol_right_;
            moving_right_ = false;
        }
    }
    else
    {
        pos_x_ -= vel_x_ * delta_time;
        if (pos_x_ < patrol_left_)
        {
            pos_x_ = patrol_left_;
            moving_right_ = true;
        }
    }

    SDL_Rect player_box = player.GetRect();
    SDL_Rect enemy_box = GetHitBox();

    int distance = abs((enemy_box.x + enemy_box.w / 2) - (player_box.x + player_box.w / 2));
    if (distance <= attack_range_)
    {
        Uint32 now = SDL_GetTicks();
        if (now - last_attack_time_ >= attack_cooldown_)
        {
            last_attack_time_ = now;
            player.TakeDamage(attack_damage_); 
        }
    }
}

void Enemy::Render(SDL_Renderer* screen, SDL_Rect* camera) 
{
    if(dead_) return;
    SDL_Rect render_pos = { (int)pos_x_ - camera->x, (int)pos_y_ - camera->y, w_, h_ };
    img_enemy_.SetRect(render_pos.x, render_pos.y);
    img_enemy_.Render(screen);
}
