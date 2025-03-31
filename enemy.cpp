#include "enemy.h"
#include "character.h"

Enemy::Enemy()
{
    img_enemy_ = nullptr;
    pos_x_ = pos_y_ = 0;
    vel_x_ = 25;
    moving_right_ = true;

    attack_range_ = 40;
    attack_cooldown_ = 2000;
    last_attack_time_ = 0;

    dead_ = false;
    death_time_ = 0;

    hp_bar_height_ = 3;
    hp_color_ = {0, 255, 0, 255};
    bg_color_ = {255, 0, 0, 255};

    detection_range_ = 200;
}

Enemy::~Enemy()
{

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

void Enemy::SetTexture(Graphic* texture)
{
    img_enemy_ = texture;
    if (texture)
    {
        SDL_Rect rect = texture->GetRect();
        w_ = rect.w;
        h_ = rect.h;
    }
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

    SDL_Rect player_box = player.GetRect();
    SDL_Rect enemy_box = GetHitBox();

    // Tính khoảng cách giữa quái và nhân vật
    int distance = abs((enemy_box.x + enemy_box.w / 2) - (player_box.x + player_box.w / 2));

    // Kiểm tra xem nhân vật có trong phạm vi tuần tra không
    bool player_in_patrol_range = (player_box.x >= patrol_left_ && player_box.x <= patrol_right_);

    if (player_in_patrol_range && distance <= detection_range_) // Nếu nhân vật trong phạm vi tuần tra và phát hiện
    {
        // Đuổi theo nhân vật nhưng không vượt ra ngoài patrol range
        if (player_box.x < enemy_box.x && pos_x_ > patrol_left_) // Nhân vật ở bên trái
        {
            pos_x_ -= vel_x_ * delta_time * 2; // Tăng tốc khi đuổi
            moving_right_ = false; // Hướng sang trái
            if (pos_x_ < patrol_left_) pos_x_ = patrol_left_; // Không vượt quá patrol_left_
        }
        else if (player_box.x > enemy_box.x && pos_x_ < patrol_right_) // Nhân vật ở bên phải
        {
            pos_x_ += vel_x_ * delta_time * 2; // Tăng tốc khi đuổi
            moving_right_ = true; // Hướng sang phải
            if (pos_x_ > patrol_right_) pos_x_ = patrol_right_; // Không vượt quá patrol_right_
        }
    }
    else // Nếu nhân vật ngoài phạm vi tuần tra hoặc không phát hiện, quay lại tuần tra
    {
        if (patrol_right_ == patrol_left_) {}
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
    }

    // Tấn công nếu trong phạm vi tấn công
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

void LoadEnemyFromFile(std::string path, std::vector<Enemy>& enemy_list, Graphic* enemy_textures)
{
    std::ifstream file(path);
    int id, x, y, l, r, hp, damage;
    while (file >> id >> x >> y >> l >> r >> hp >> damage)
    {
        Enemy e;
        e.SetId(id);
        e.SetPosition(x, y);
        e.SetPatrolRange(l, r);
        e.SetHP(hp);
        e.SetDamage(damage);
        enemy_list.push_back(e);
    }

    // Gán texture từ mảng enemy_textures dựa trên chỉ số
    enemy_textures[0].LoadImg("img/thucthao.png", g_render);
    enemy_textures[1].LoadImg("img/ocma.png", g_render);
    enemy_textures[2].LoadImg("img/khi.png", g_render);
    enemy_textures[3].LoadImg("img/oc.png", g_render);
    enemy_textures[4].LoadImg("img/heo.png", g_render);
    int c = -1;
    for (int i = 1; i <= 5; i++) // 5 thucthao
        enemy_list[++c].SetTexture(&enemy_textures[0]);
    for (int i = 1; i <= 4; i++) // 4 ocma
        enemy_list[++c].SetTexture(&enemy_textures[1]);
    for (int i = 1; i <= 6; i++) // 6 khi
        enemy_list[++c].SetTexture(&enemy_textures[2]);
    for (int i = 1; i <= 4; i++) // 4 oc
        enemy_list[++c].SetTexture(&enemy_textures[3]);
    enemy_list[++c].SetTexture(&enemy_textures[4]); // 1 heo
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

void Enemy::RenderHPBar(SDL_Renderer* screen, SDL_Rect* camera) {
    if (dead_ || hp_ <= 0) return;

    int hp_bar_x = (int)pos_x_ - camera->x;
    int hp_bar_y = (int)pos_y_ - camera->y - hp_bar_height_ - 2; 

    SDL_Rect bg_rect = { hp_bar_x, hp_bar_y, w_, hp_bar_height_ };
    SDL_SetRenderDrawColor(screen, bg_color_.r, bg_color_.g, bg_color_.b, bg_color_.a);
    SDL_RenderFillRect(screen, &bg_rect);

    float hp_ratio = (float)hp_ / max_hp_;
    int current_hp_width = (int)(w_ * hp_ratio);

    SDL_Rect hp_rect = { hp_bar_x, hp_bar_y, current_hp_width, hp_bar_height_ };
    SDL_SetRenderDrawColor(screen, hp_color_.r, hp_color_.g, hp_color_.b, hp_color_.a);
    SDL_RenderFillRect(screen, &hp_rect);
}

void Enemy::Render(SDL_Renderer* screen, SDL_Rect* camera) 
{
    if(dead_) return;
    SDL_Rect render_pos = { (int)pos_x_ - camera->x, (int)pos_y_ - camera->y, w_, h_ };
    img_enemy_->SetRect(render_pos.x, render_pos.y);
    img_enemy_->Render(screen);

    RenderHPBar(screen, camera);
}
