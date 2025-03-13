#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "commonFunc.h"

class Graphic
{
public:
    Graphic();
    ~Graphic();
    void SetRect(const int& x, const int& y)  {rect_.x = x; rect_.y = y;}
    SDL_Rect GetRect() const {return rect_;}
    SDL_Texture* GetObject() const {return p_object_;}

    bool LoadImg(std::string path, SDL_Renderer* screen);
    void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
    void Free();

protected:
    SDL_Texture* p_object_;
    SDL_Rect rect_;
};


#endif