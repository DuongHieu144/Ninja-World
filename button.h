#ifndef BUTTON_H
#define BUTTON_H

#include "graphic.h"
#include <string>

class Button 
{
public:
    Button();
    ~Button();
    void LoadImg(const std::string& path, SDL_Renderer* renderer); 
    void SetPosition(int x, int y);
    void Render(SDL_Renderer* renderer);
    bool IsClicked(int mouse_x, int mouse_y) const;
    SDL_Rect GetRect() const { return rect_; }

private:
    SDL_Rect rect_;         
    Graphic texture_;       
};

#endif