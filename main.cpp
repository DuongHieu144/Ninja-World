#include "commonFunc.h"
#include "graphic.h"
#include "character.h"
#include "map.h"
#include "enemy.h"

SDL_Window* g_window = NULL;
SDL_Renderer* g_render = NULL;
TTF_Font* g_font = NULL;
SDL_Event g_event;

Graphic g_background; 
Character g_character; 
std::vector<std::vector<int> > map_data;
std::vector<Enemy> enemy_list;
Enemy enemy1;

SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

bool InitData() 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Ninja Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!g_window) return false;
    g_render = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_render) return false;
    SDL_SetRenderDrawColor(g_render, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    if (TTF_Init() == -1) return false;
    g_font = TTF_OpenFont("arial.ttf", 20);
    return true;
}

bool LoadResources() {
    if (!g_background.LoadImg("img/background.png", g_render)) return false;
    if (!g_character.LoadImg("img/character.png", g_render)) return false;
    LoadMapFromFile("map2.txt", map_data);
    return true;
}

bool LoadEnemy()
{
    if(!enemy1.LoadImg("img/enemy.png", g_render)) return false;
    enemy1.SetPosition(100, 100);
    enemy1.SetHP(100);
    enemy_list.push_back(enemy1);
    return true;
}

void UpdateCamera() {
    SDL_Rect player_rect = g_character.GetRect();
    camera.x = player_rect.x - SCREEN_WIDTH / 2;
    camera.y = player_rect.y - SCREEN_HEIGHT / 2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x + camera.w > map_width) camera.x = map_width - camera.w;
    if (camera.y + camera.h > map_height) camera.y = map_height - camera.h;
}

void Close() {
    g_background.Free();
    g_character.~Character(); 
    SDL_DestroyRenderer(g_render);
    SDL_DestroyWindow(g_window);
    TTF_CloseFont(g_font);
    g_render = NULL;
    g_window = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) 
{
    if (!InitData()) return -1;
    if (!LoadResources()) return -1;
    if (!LoadEnemy()) return -1;
    bool is_quit = false;

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = now;
    double delta_time = 0.0;
    while (!is_quit) {
        last = now;
    now = SDL_GetPerformanceCounter();
    delta_time = (double)(now - last) / SDL_GetPerformanceFrequency();
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) {
                is_quit = true;
            }
            g_character.HandleInput(g_event);
        }

        g_character.Move(delta_time, map_data);
        UpdateCamera();

        SDL_SetRenderDrawColor(g_render, 0, 0, 0, 255);
        SDL_RenderClear(g_render);

        SDL_Rect src_rect = {camera.x, camera.y, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect dst_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(g_render, g_background.GetObject(), &src_rect, &dst_rect);

        g_character.Render(g_render, &camera);
        g_character.ShowPosition(g_render, g_font, &camera);
        enemy1.Render(g_render, &camera);
        SDL_RenderPresent(g_render);
    }

    Close();
    return 0;
}