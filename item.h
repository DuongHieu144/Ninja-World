#ifndef ITEM_H
#define ITEM_H

#include "graphic.h"

class Item 
{
    public:
        void Render(SDL_Renderer* renderer, SDL_Rect* camera);
        SDL_Rect GetPosition() const {return{pos_x_, pos_y_, 16, 16};}
        void SetPosition(double x, double y) { pos_x_ = x; pos_y_ = y;}
        void SetId(int id);
        int GetId() const {return id_;}
    
    private:
        int id_; 
        int pos_x_, pos_y_;
        int width_ = 16, height_ = 16; 
    };
    
#endif