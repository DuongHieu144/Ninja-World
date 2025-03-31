#include "item.h"

void Item::Render(SDL_Renderer* renderer, SDL_Rect* camera)
{
    if (id_ == 1) 
    {
        item_hp.LoadImg("img/hp.png", renderer);
        item_hp.SetRect((int)pos_x_ - camera->x - 10, (int)pos_y_ - camera->y );
        item_hp.Render(renderer);
        item_hp.Free();
    }
    else if (id_ == 2) 
    {
        item_hp.LoadImg("img/hp.png", renderer);
        item_mp.SetRect((int)pos_x_ - camera->x - 10, (int)pos_y_ - camera->y );
        item_mp.Render(renderer);
        item_mp.Free();
    }
   
}

void Item::SetId(int id)
{
    id_ = id;
}