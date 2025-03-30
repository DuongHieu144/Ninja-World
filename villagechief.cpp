#include "villagechief.h"

void VillageChief::SetQuests(std:: vector<Quest*> quest_list)
{
    quests_ = quest_list;
}

void VillageChief::Interact(Character& player)
{
    int quest_stage = player.GetQuestState();
    if(quest_stage > quests_.size())
    {
        return;
    }

    Quest* quest = quests_[quest_stage];
    if(player.GetCurrentQuest() == nullptr) 
    {
        player.SetQuest(quest);
        std::cout<<quest_stage;
        return;
    }
    if(quest->IsCompleted())
    {
        player.QuestReward();
        player.NextQuest();
    }
    else if(!quest->IsCompleted())
    {
    }
}