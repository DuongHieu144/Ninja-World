#ifndef ENEMY_H
#define ENEMY_H

#include "commonFunc.h"
#include "graphic.h"

class Enemy
{
public:
    Enemy();
    ~Enemy();

    void SetPosition(int x, int y);
    void SetHP(int hp);
    void TakeDamage(int damage);
    bool IsDead() const;

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* screen, SDL_Rect* camera);
    SDL_Rect GetHitBox() const;

private:
    Graphic img_enemy_;
    int pos_x_;
    int pos_y_;
    int hp_;
    int max_hp_;
    int w_, h_;
};

#endif