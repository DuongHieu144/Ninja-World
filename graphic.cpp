#include "graphic.h"
Graphic::Graphic()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;

}

Graphic::~Graphic()
{
    Free();
}

bool Graphic::LoadImg(std::string path, SDL_Renderer* screen)
{
    SDL_Texture* new_texture = NULL;

    SDL_Surface* load_surface = IMG_Load(path.c_str());
    if(load_surface != NULL)
    {
        SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
        new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
        if(new_texture != NULL)
        {
            rect_.w = load_surface->w;
            rect_.h = load_surface->h;
        }
        SDL_FreeSurface(load_surface);
    }
    p_object_ = new_texture;

    return p_object_ !=NULL;
}

void Graphic::Render(SDL_Renderer* des, const SDL_Rect* clip, const SDL_Rect* dst, SDL_RendererFlip flip)
{
    if (dst == NULL)
    {
        SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
        SDL_RenderCopyEx(des, p_object_, clip, &renderquad, 0.0, nullptr, flip);
    }
    else 
    {
        SDL_RenderCopyEx(des, p_object_, clip, dst, 0.0, nullptr, flip);
    }
}

void Graphic::Free()
{
    if(p_object_ !=NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}