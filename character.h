#ifndef CHARACTER_H
#define CHARACTER_H

#include "graphic.h"
#include "vector"

class Character
{
public:
    Character();
    ~Character();

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void HandleInput(SDL_Event& event);
    void Move(double delta_time, std::vector<std::vector<int> >& map_data);
    void Render(SDL_Renderer* des, SDL_Rect* camera);
    void ShowPosition(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* camera);
    bool CheckCollision(int x, int y, std::vector<std::vector<int> >& map_data);
    SDL_Rect GetRect() const { return player_.GetRect(); }

private:
    Graphic player_;
    double pos_x_, pos_y_;
    double vel_x_, vel_y_;
    double gravity_y;
    double gravity_x;
    double jump;
    const double SPEED = 150.0;
    double target_speed_x_ = 0.0;
    int h_player = 38;
    int w_player = 21;
    bool on_ground_;
    bool flag_right_;
    bool flag_left_;
};

#endif
