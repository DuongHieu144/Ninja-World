#ifndef VILLAGE_CHIEF_H
#define VILLAGE_CHIEF_H

#include "character.h"
#include "quest.h"
#include <vector>

class VillageChief 
{
public:
    void Interact(Character& player);
    void SetQuests(std::vector<Quest*> quest_list);

private:
    std::vector<Quest*> quests_;
    Graphic npc_img_;
};

#endif
