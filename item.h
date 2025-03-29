#ifndef ITEM_H
#define ITEM_H

#include "graphic.h"

class Item 
{
    public:
        void Render(SDL_Renderer* renderer, SDL_Rect* camera);
        
        void SetPosition(double x, double y) { pos_x_ = x; pos_y_ = y; }
        void SetId(int id);
    
    private:
        int id_; 
        int pos_x_, pos_y_;
        int width_ = 16, height_ = 16; 
        Graphic item_hp;
        Graphic item_mp;
        Graphic item_nv1;
        Graphic item_nv2;
    };
    
#endif