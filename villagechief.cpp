#include "villagechief.h"

VillageChief::VillageChief(SDL_Renderer* renderer) 
{
    // Đặt vị trí cho các nút
    get_quest_button_.SetPosition(SCREEN_WIDTH / 2 - 72, SCREEN_HEIGHT / 2 + 150);
    cancel_button_.SetPosition(SCREEN_WIDTH / 2 + 16, SCREEN_HEIGHT / 2 + 150);
    complete_button_.SetPosition(SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 +150);
}

void VillageChief::SetQuests(std::vector<Quest*> quest_list) 
{
    quests_ = quest_list;
}

// Tương tác với nhân vật
void VillageChief::Interact(Character& player, SDL_Event& event, bool& show_menu, SDL_Rect& player_box) 
{
    int quest_stage = player.GetQuestState();
    if (quest_stage >= quests_.size()) return;

    Quest* quest = quests_[quest_stage];

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && !show_menu && player.GetState() == 0 && (player.GetCurrentQuest() == nullptr || quest->IsCompleted())) 
    {
        if (SDL_HasIntersection(&player_box, &npc_box_)) 
        {
            show_menu = true;
        }
    }

    if (show_menu && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) 
    {
        int mouse_x = event.button.x;
        int mouse_y = event.button.y;

        if (player.GetCurrentQuest() == nullptr) 
        {
            // Chưa nhận nhiệm vụ
            if (get_quest_button_.IsClicked(mouse_x, mouse_y)) 
            {
                player.SetQuest(quest);
                show_menu = false;
            } 
            else if (cancel_button_.IsClicked(mouse_x, mouse_y)) 
            {
                show_menu = false;
            }
        } 
        else if (quest->IsCompleted()) 
        {
            // Đã hoàn thành nhiệm vụ
            if (complete_button_.IsClicked(mouse_x, mouse_y)) 
            {
                player.QuestReward();
                player.NextQuest();
                show_menu = false;
            }
        }
    }
}

void VillageChief::RenderMenu(SDL_Renderer* renderer, Character& player) 
{
    if (quests_.empty() || quests_.size() <= player.GetQuestState()) return;
    get_quest_button_.LoadImg("img/get_quest_button.png", renderer);
    cancel_button_.LoadImg("img/cancel_button.png", renderer);
    complete_button_.LoadImg("img/complete_button.png", renderer);

    Quest* current_quest = quests_[player.GetQuestState()];
    if (player.GetCurrentQuest() == nullptr) 
    {
        // Hiển thị nút "Get Quest" và "Cancel"
        get_quest_button_.Render(renderer);
        cancel_button_.Render(renderer);
    } 
    else if (current_quest->IsCompleted()) 
    {
        // Hiển thị nút "Complete"
        complete_button_.Render(renderer);
    }
    get_quest_button_.~Button();
    cancel_button_.~Button();
    complete_button_.~Button();
}