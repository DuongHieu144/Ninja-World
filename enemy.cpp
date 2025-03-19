#include "enemy.h"

Enemy::Enemy()
{
    pos_x_ = pos_y_ = 0;

}

Enemy::~Enemy()
{
    img_enemy_.Free();
}

void Enemy::SetPosition(int x, int y)
{
    pos_x_ = x;
    pos_y_ = y;
}

void Enemy::SetHP(int hp)
{
    hp_ = max_hp_ = hp;
}

void Enemy::TakeDamage(int damage)
{
    hp_ -= damage;
    if(hp_ < 0) hp_ = 0;
}

bool Enemy::IsDead() const
{
    return hp_ <= 0;
}

bool Enemy::LoadImg(std::string path, SDL_Renderer* screen) 
{
    if (!img_enemy_.LoadImg(path, screen)) return false;
    SDL_Rect rect = img_enemy_.GetRect();
    w_ = rect.x;
    h_ = rect.y;
    return true;
}

void Enemy::Render(SDL_Renderer* screen, SDL_Rect* camera) {
    SDL_Rect render_pos = { pos_x_ - camera->x, pos_y_ - camera->y, w_, h_ };
    img_enemy_.SetRect(render_pos.x, render_pos.y);
    img_enemy_.Render(screen);
}

SDL_Rect Enemy::GetHitBox() const 
{
    return { pos_x_, pos_y_, w_, h_ };
}
