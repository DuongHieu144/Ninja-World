#ifndef ENEMY_H
#define ENEMY_H

#include "commonFunc.h"
#include "graphic.h"
#include <vector>

class Character;

class Enemy
{
public:
    Enemy();
    ~Enemy();

    void SetPosition(double x, double y);
    void SetHP(int hp);
    void SetDamage(int damage);
    void TakeDamage(int damage,Character& player);
    void SetPatrolRange(int left, int right);
    bool IsDead() const;

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* screen, SDL_Rect* camera);
    void Update(double delta_time, Character& player);
    SDL_Rect GetHitBox() const;

    void RespawnIfNeeded();

private:
    Graphic img_enemy_;
    double pos_x_;
    double pos_y_;
    double vel_x_;
    int patrol_left_, patrol_right_;
    bool moving_right_;

    int hp_;
    int max_hp_;
    int attack_range_;
    int attack_damage_;
    Uint32 last_attack_time_;
    Uint32 attack_cooldown_;

    bool dead_;
    Uint32 death_time_;

    int w_, h_;

};

void LoadEnemyFromFile(std::string path, std::vector<Enemy>& enemy_list);

#endif