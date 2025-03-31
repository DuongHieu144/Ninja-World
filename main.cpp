#include "commonFunc.h"
#include "graphic.h"
#include "character.h"
#include "map.h"
#include "enemy.h"
#include "villagechief.h"
#include "item.h"
#include "button.h"

SDL_Window* g_window = NULL;
SDL_Renderer* g_render = NULL;
TTF_Font* g_font = NULL;
SDL_Event g_event;

Graphic g_background; 
Character g_character; 
std::vector<std::vector<int> > map_data;
std::vector<Enemy> enemy_list;
VillageChief village_npc(g_render);
std::vector<Item> item_list;

Quest quest1("Kill 3 roses", 3, 1);  
Quest quest2("Kill 5 purple snails", 5, 2);   
Quest quest3("Kill 7 monkeys", 7, 3);  
Quest quest4("Kill 10 snails", 10, 4);
Quest quest5("Collect martial arts secrets from the boss", 1, 5);

bool show_menu = false;
bool menu_active = false;
int mouse_x = 0, mouse_y = 0;

SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
Graphic enemy_textures[5];

Graphic main_menu_background;
Button new_game_button;
Graphic game_over_background;
Graphic winner_background;

Mix_Music* g_background_music = NULL;
Mix_Chunk* g_attack_sound = NULL;

enum GameState 
{
    MENU,
    PLAYING,
    GAME_OVER,
    WINNER
};

bool InitData() 
{
    if (SDL_Init(SDL_INIT_VIDEO) | SDL_INIT_AUDIO < 0) return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    g_window = SDL_CreateWindow("Ninja Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!g_window) return false;
    g_render = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_render) return false;
    SDL_SetRenderDrawColor(g_render, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    if (TTF_Init() == -1) return false;
    g_font = TTF_OpenFont("arial.ttf", 20);
    if (!g_font) return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;
    return true;
}

bool LoadResources() 
{
    if (!g_background.LoadImg("img/map.png", g_render)) return false;
    if (!g_character.LoadImg(g_render)) return false;
    if (!main_menu_background.LoadImg("img/background.png", g_render)) return false;
    new_game_button.LoadImg("img/new_game_button.png", g_render);
    new_game_button.SetPosition(SCREEN_WIDTH/2 - 52, SCREEN_HEIGHT - 100);
    if (!game_over_background.LoadImg("img/game_over.png", g_render)) return false;
    if (!winner_background.LoadImg("img/winner_background.png", g_render)) return false;
    LoadMapFromFile("map.txt", map_data);
    LoadEnemyFromFile("enemy.txt", enemy_list, enemy_textures);
    g_background_music = Mix_LoadMUS("audio/background_music.mp3");
    if (!g_background_music) return false;
    g_attack_sound = Mix_LoadWAV("audio/attack_sound.wav");
    if (!g_attack_sound) return false;
    Enemy::LoadBloodSplashTexture(g_render);
    return true;
}

void UpdateCamera() 
{
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
    for (int i = 0; i < 5; i++) 
    {
        enemy_textures[i].Free();
    }
    main_menu_background.Free();
    new_game_button.~Button();
    game_over_background.Free();
    winner_background.Free();

    // Giải phóng âm thanh
    Mix_FreeMusic(g_background_music);
    Mix_FreeChunk(g_attack_sound);
    g_background_music = NULL;
    g_attack_sound = NULL;
    Mix_Quit();

    SDL_DestroyRenderer(g_render);
    SDL_DestroyWindow(g_window);
    TTF_CloseFont(g_font);
    g_render = NULL;
    g_window = NULL;
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void ResetGame() 
{
    g_character.Reset();
    enemy_list.clear();
    item_list.clear();
    LoadEnemyFromFile("enemy.txt", enemy_list, enemy_textures);
    village_npc.SetQuests({&quest1, &quest2, &quest3, &quest4, &quest5});
    bool tmp = g_character.LoadImg(g_render);
}

void RenderMainMenu() 
{
    SDL_SetRenderDrawColor(g_render, 0, 0, 0, 255);
    SDL_RenderClear(g_render);
    main_menu_background.Render(g_render);
    new_game_button.Render(g_render);
}

bool HandleMenuInput(SDL_Event& event) 
{
    if (event.type == SDL_MOUSEBUTTONDOWN) 
    {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        if (new_game_button.IsClicked(mouse_x, mouse_y)) 
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) 
{
    if (!InitData()) return -1;
    if (!LoadResources()) return -1;

    GameState game_state = MENU;
    bool is_quit = false;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = now;
    double delta_time = 0.0;
    Uint32 state_change_time = 0;

    village_npc.SetQuests({&quest1, &quest2, &quest3, &quest4, &quest5});

    while (!is_quit) 
    {
        last = now;
        now = SDL_GetPerformanceCounter();
        delta_time = (double)(now - last) / SDL_GetPerformanceFrequency();
        SDL_Rect player_box = g_character.GetPosition();

        while (SDL_PollEvent(&g_event) != 0) 
        {
            if (g_event.type == SDL_QUIT) 
            {
                is_quit = true;
            }
            SDL_GetMouseState(&mouse_x, &mouse_y);

            switch (game_state) {
                case MENU:
                    if (HandleMenuInput(g_event)) {
                        game_state = PLAYING;
                        ResetGame();
                        Mix_PlayMusic(g_background_music, -1);
                        Mix_VolumeMusic(64);
                        Mix_VolumeChunk(g_attack_sound, 124);
                    }
                    break;
                case PLAYING:
                    if (!menu_active) g_character.HandleInput(g_event, enemy_list, g_character, item_list);
                    village_npc.Interact(g_character, g_event, show_menu, menu_active, player_box);
                    break;
                case GAME_OVER:
                case WINNER:
                    break;
            }
        }

        // Di chuyển các biến ra ngoài switch để tránh lỗi bypass initialization
        std::string quest_text;  // Khai báo ở đây thay vì trong case
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;

        switch (game_state) {
            case MENU:
            {
                RenderMainMenu();
                SDL_RenderPresent(g_render);
                break;
            }
            case PLAYING:
            {
                g_character.UpdateAnimation();
                g_character.UpdateAttackAnimation();
                g_character.Move(delta_time, map_data);
                for(auto& x : enemy_list)
                {
                    x.Update(delta_time, g_character);
                    x.Respawn();
                }
   
                UpdateCamera();

                SDL_SetRenderDrawColor(g_render, 0, 0, 0, 255);
                SDL_RenderClear(g_render);

                SDL_Rect src_rect = {camera.x, camera.y, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_Rect dst_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderCopy(g_render, g_background.GetObject(), &src_rect, &dst_rect);

                g_character.Render(g_render, &camera);
                g_character.ShowPotion(g_render, g_font, &camera);
                for(auto& x : enemy_list) x.Render(g_render, &camera);
                for(auto& x : item_list) x.Render(g_render, &camera);

                int i_item = -1;
                for(int i = 0; i < item_list.size(); i++)
                {
                    SDL_Rect item_box = item_list[i].GetPosition();
                    if(SDL_HasIntersection(&player_box, &item_box))
                    {
                        i_item = i;
                        g_character.PickUpItem(item_list[i].GetId());
                        break;
                    }
                }
                if(i_item != -1) item_list.erase(item_list.begin() + i_item);

                quest_text = g_character.GetCurrentQuestInfo();
                SDL_Color textColor = {255, 255, 0};
                surface = TTF_RenderText_Solid(g_font, quest_text.c_str(), textColor);
                if(surface != nullptr)
                {
                    texture = SDL_CreateTextureFromSurface(g_render, surface);
                    SDL_Rect text_rect = {20, 200, surface->w, surface->h}; 
                    SDL_RenderCopy(g_render, texture, NULL, &text_rect);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                }
                if (show_menu) 
                {
                    village_npc.RenderMenu(g_render, g_character);
                }

                if (g_character.GetHP() <= 0) {
                    game_state = GAME_OVER;
                    state_change_time = SDL_GetTicks();
                    Mix_PauseMusic();
                }
                if (g_character.GetQuestState() == 5 && quest5.IsCompleted()) {
                    game_state = WINNER;
                    state_change_time = SDL_GetTicks();
                    Mix_PauseMusic();
                }

                SDL_RenderPresent(g_render);
                break;
            }
            case GAME_OVER:
            {
                SDL_SetRenderDrawColor(g_render, 0, 0, 0, 255);
                SDL_RenderClear(g_render);
                game_over_background.Render(g_render);
                SDL_RenderPresent(g_render);
                if (SDL_GetTicks() - state_change_time >= 5000) {
                    game_state = MENU;
                }
                break;
            }
            case WINNER:
            {
                SDL_SetRenderDrawColor(g_render, 0, 0, 0, 255);
                SDL_RenderClear(g_render);
                winner_background.Render(g_render);
                SDL_RenderPresent(g_render);
                if (SDL_GetTicks() - state_change_time >= 60000) {
                    game_state = MENU;
                }
                break;
            }
        }
    }

    Close();
    return 0;
}