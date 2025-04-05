#ifndef VILLAGE_CHIEF_H
#define VILLAGE_CHIEF_H

#include "character.h"
#include "quest.h"
#include "button.h"
#include <vector>

class VillageChief 
{
public:
VillageChief(SDL_Renderer* renderer);
void SetQuests(std::vector<Quest*> quest_list);
void Interact(Character& player, SDL_Event& event, bool& show_menu, bool& menu_active, SDL_Rect& player_box);
void RenderMenu(SDL_Renderer* renderer, Character& player);

private:
    std::vector<Quest*> quests_;
    Graphic npc_img_;
    Button get_quest_button_;
    Button cancel_button_;
    Button complete_button_;
    SDL_Rect npc_box_ = {300, 200, 300, 150};
};

#endif
