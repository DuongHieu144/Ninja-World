#ifndef QUEST_H
#define QUEST_H

#include <string>

class Quest 
{
public:
    Quest(std::string desc, int goal, int reward_type)
        : description(desc), goal(goal), progress(0), 
          completed(false), reward_type(reward_type) {}

    void InCurrent() 
    {
        if (!completed && progress < goal) 
        {
            progress++;
            if (progress >= goal)
                completed = true;
        }
    }

    bool IsCompleted() const { return completed; }
    std::string GetDescription() const { return description; }
    int GetProgress() const { return progress; }
    int GetGoal() const { return goal; }
    int GetRewardType() const { return reward_type; }

private:
    std::string description;
    int goal;
    int progress;
    bool completed;
    int reward_type;  
};

#endif
