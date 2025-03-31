#include "item.h"

void Item::Render(SDL_Renderer* renderer, SDL_Rect* camera)
{
    if (id_ == 1) 
    {
        Graphic img_potion;
        img_potion.LoadImg("img/hp.png", renderer);
        img_potion.SetRect((int)pos_x_ - camera->x - 10, (int)pos_y_ - camera->y );
        img_potion.Render(renderer);
        img_potion.Free();
    }
    else if (id_ == 2) 
    {
        Graphic img_potion;
        img_potion.LoadImg("img/hp.png", renderer);
        img_potion.SetRect((int)pos_x_ - camera->x - 10, (int)pos_y_ - camera->y );
        img_potion.Render(renderer);
        img_potion.Free();
    }
   
}

void Item::SetId(int id)
{
    id_ = id;
}