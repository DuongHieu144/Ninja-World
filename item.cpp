#include "item.h"

void Item::Render(SDL_Renderer* renderer, SDL_Rect* camera)
{
    if(id_ == 1) 
    {
        bool tmp1 = item_hp.LoadImg("img/hp.png", renderer);
        item_hp.SetRect((int)pos_x_ - camera->x, (int)pos_y_ - camera->y+10);
        item_hp.Render(renderer);
    }
    if(id_ == 2) 
    {
        bool tmp1 = item_mp.LoadImg("img/mp.png", renderer);
        item_mp.SetRect((int)pos_x_ - camera->x, (int)pos_y_ - camera->y+10);
        item_mp.Render(renderer);
    }
    if(id_ == 3) 
    {
        bool tmp1 = item_nv1.LoadImg("img/vo_oc.png", renderer);
        item_nv1.SetRect((int)pos_x_ - camera->x, (int)pos_y_ - camera->y+10);
        item_nv1.Render(renderer);
    }
    if(id_ == 4) 
    {
        bool tmp1 = item_nv2.LoadImg("img/bi_kip.png", renderer);
        item_nv2.SetRect((int)pos_x_ - camera->x, (int)pos_y_ - camera->y+10);
        item_nv2.Render(renderer);
    }
}

void Item::SetId(int id)
{
    id_ = id;
}