#ifndef CHARACTER_H
#define CHARACTER_H

#include "graphic.h"

class Character
{
public:
    Character();
    ~Character();

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void HandleInput(SDL_Event& event);
    void Move(double delta_time);
    void Render(SDL_Renderer* des, SDL_Rect* camera);
    SDL_Rect GetRect() const { return player_.GetRect(); }

private:
    Graphic player_;
    double pos_x_, pos_y_;
    double vel_x_, vel_y_;
    double gravity_;
    double max_fall_;
    double jump;
    const double SPEED = 200.0;
    bool on_ground_;
    bool CheckCollision(int x, int y);
};

#endif
