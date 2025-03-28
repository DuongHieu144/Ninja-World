#include "commonFunc.h"
#include "graphic.h"
#include "character.h"
#include "map.h"
#include "enemy.h"
#include "villagechief.h"

SDL_Window* g_window = NULL;
SDL_Renderer* g_render = NULL;
TTF_Font* g_font = NULL;
SDL_Event g_event;

Graphic g_background; 
Character g_character; 
std::vector<std::vector<int> > map_data;
std::vector<Enemy> enemy_list;
VillageChief village_npc;

// Danh sách nhiệm vụ
Quest quest1("Kill 3 enemies", 3, 1);  // Tăng HP +20
Quest quest2("Kill 5 enemies", 5, 2);   // Tăng damage +5
Quest quest3("Kill 7 enemies", 7, 3);   // Mở khóa kỹ năng


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
    if (!g_background.LoadImg("img/map.png", g_render)) return false;
    if (!g_character.LoadImg("img/character.png", g_render)) return false;
    LoadMapFromFile("map.txt", map_data);
    return true;
}

bool LoadEnemy()
{
    LoadEnemyFromFile("enemy.txt", enemy_list);
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
    village_npc.SetQuests({&quest1, &quest2, &quest3});
    while (!is_quit) 
    {
        last = now;
        now = SDL_GetPerformanceCounter();
        delta_time = (double)(now - last) / SDL_GetPerformanceFrequency();
        while (SDL_PollEvent(&g_event) != 0) 
        {
            if (g_event.type == SDL_QUIT) 
            {
                is_quit = true;
            }
            g_character.HandleInput(g_event, enemy_list, g_character);
            if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_SPACE)
            {
                SDL_Rect player_box = g_character.GetRect();
                SDL_Rect npc_box = {300, 200, 300, 300};
                if (SDL_HasIntersection(&player_box, &npc_box)) 
                    village_npc.Interact(g_character);
            }
        }   

        g_character.Move(delta_time, map_data);
        for(auto& x : enemy_list)
        {
            x.Update(delta_time, g_character);
            x.RespawnIfNeeded();
        }
        
        UpdateCamera();

        SDL_SetRenderDrawColor(g_render, 0, 0, 0, 255);
        SDL_RenderClear(g_render);

        SDL_Rect src_rect = {camera.x, camera.y, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect dst_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(g_render, g_background.GetObject(), &src_rect, &dst_rect);

        g_character.Render(g_render, &camera);
        g_character.ShowPosition(g_render, g_font, &camera);
        for(auto&x : enemy_list) x.Render(g_render, &camera);

            // Hiển thị nhiệm vụ dọc theo biên trên của màn hình
        std::string quest_text = g_character.GetCurrentQuestInfo();
        SDL_Color textColor = {255, 255, 0}; // Màu chữ vàng

        SDL_Surface* surface = TTF_RenderText_Solid(g_font, quest_text.c_str(), textColor);
        if(surface != nullptr)
        {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(g_render, surface);
            SDL_Rect text_rect = {20, 10, surface->w, surface->h}; // Căn chỉnh vị trí trên cùng màn hình
            SDL_RenderCopy(g_render, texture, NULL, &text_rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
        SDL_RenderPresent(g_render);
    }

    Close();
    return 0;
}