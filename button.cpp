#include "button.h"

Button::Button() 
{
    rect_ = {0, 0, 64, 64};
}

Button::~Button() { texture_.Free(); }

void Button::LoadImg(const std::string& path, SDL_Renderer* renderer) 
{
    texture_.LoadImg(path.c_str(), renderer);
}

void Button::SetPosition(int x, int y) 
{
    rect_.x = x;
    rect_.y = y;
}

void Button::Render(SDL_Renderer* renderer) 
{
    texture_.SetRect(rect_.x, rect_.y);
    texture_.Render(renderer);
}

bool Button::IsClicked(int mouse_x, int mouse_y) const 
{
    return (mouse_x >= rect_.x && mouse_x <= rect_.x + rect_.w &&
            mouse_y >= rect_.y && mouse_y <= rect_.y + rect_.h);
}