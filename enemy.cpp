#include "enemy.h"
#include "character.h"
#include "item.h"

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

void Enemy::SetId(int id)
{
    id_ = id;
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

void Enemy::TakeDamage(int damage, Character& player, std::vector<Item>& items)
{
    if (!dead_)
    {
        hp_ -= damage;
        if (hp_ <= 0)
        {
            player.DoQuest(id_);
            hp_ = 0;
            dead_ = true;
            death_time_ = SDL_GetTicks();
            DropItem(items);
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


void Enemy::Respawn()
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

void LoadEnemyFromFile(std::string path, std::vector<Enemy>& enemy_list)
{
    std::ifstream file(path);
    int id, x, y, l, r, hp, damage;
    while(file>>id>>x>>y>>l>>r>>hp>>damage)
    {
        Enemy e;
        e.SetId(id);
        e.SetPosition(x, y);
        e.SetPatrolRange(l, r);
        e.SetHP(hp);
        e.SetDamage(damage);
        enemy_list.push_back(e);
    }
    std::string e1, e2, e3, e4, e5;
    e1 = "img/thucthao.png";
    e2 = "img/ocma.png";
    e3 = "img/khi.png";
    e4 = "img/oc.png";
    int c=-1;
    for(int i=1; i<=5; i++)
        bool tmp = enemy_list[++c].LoadImg(e1, g_render);
    for(int i=1; i<=4; i++)
        bool tmp = enemy_list[++c].LoadImg(e2, g_render);
    for(int i=1; i<=6; i++)
        bool tmp = enemy_list[++c].LoadImg(e3, g_render);
    for(int i=1; i<=4; i++)
        bool tmp = enemy_list[++c].LoadImg(e4, g_render);
    
}

void Enemy::DropItem(std::vector<Item>& items)
{
    int drop_chance = rand() % 100;
    Item new_item;
    if(drop_chance <=40)
    {
        new_item.SetId(1);
    }
    else if(drop_chance <=80)
    {
        new_item.SetId(2);
    }
    new_item.SetPosition(pos_x_, pos_y_);
    items.push_back(new_item);

}

void Enemy::Render(SDL_Renderer* screen, SDL_Rect* camera) 
{
    if(dead_) return;
    SDL_Rect render_pos = { (int)pos_x_ - camera->x, (int)pos_y_ - camera->y, w_, h_ };
    img_enemy_.SetRect(render_pos.x, render_pos.y);
    img_enemy_.Render(screen);
}
