#ifndef ENEMY_H
#define ENEMY_H

#include "commonFunc.h"
#include "graphic.h"
#include <vector>
#include "item.h"

class Character;

class Enemy
{
public:
    Enemy();
    ~Enemy();

    void SetId(int id);
    void SetPosition(double x, double y);
    void SetHP(int hp);
    void SetDamage(int damage);
    void TakeDamage(int damage,Character& player, std::vector<Item>& items);
    void SetPatrolRange(int left, int right);
    bool IsDead() const;

    void SetTexture(Graphic* texture);
    void Render(SDL_Renderer* screen, SDL_Rect* camera);
    void RenderHPBar(SDL_Renderer* screen, SDL_Rect* camera);
    void Update(double delta_time, Character& player);
    SDL_Rect GetHitBox() const;

    void Respawn();
    void DropItem(std::vector<Item>& items);

    static void LoadBloodSplashTexture(SDL_Renderer* renderer);

private:
    Graphic* img_enemy_;
    double pos_x_;
    double pos_y_;
    double vel_x_;
    int patrol_left_, patrol_right_;
    bool moving_right_;

    int id_;
    int hp_;
    int max_hp_;
    int attack_range_;
    int attack_damage_;
    Uint32 last_attack_time_;
    Uint32 attack_cooldown_;

    bool dead_;
    Uint32 death_time_;

    int w_, h_;
  
    int hp_bar_height_;   
    SDL_Color hp_color_;  
    SDL_Color bg_color_;

    int detection_range_;

    // Biến cho hiệu ứng tóe máu
    static Graphic blood_splash_texture_; // Texture tóe máu dùng chung cho tất cả Enemy
    Uint32 blood_splash_start_time_;      // Thời gian bắt đầu hiệu ứng
    bool show_blood_splash_;              // Trạng thái hiển thị hiệu ứng
    const Uint32 BLOOD_SPLASH_DURATION = 200; // Thời gian hiển thị (ms), ví dụ: 200ms
};

void LoadEnemyFromFile(std::string path, std::vector<Enemy>& enemy_list, Graphic* enemy_textures);

#endif