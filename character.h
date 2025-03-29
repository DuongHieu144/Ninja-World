#ifndef CHARACTER_H
#define CHARACTER_H

#include "graphic.h"
#include "vector"
#include "quest.h"
#include "item.h"

class Enemy;

class Character
{
public:
    Character();
    ~Character();

    void HandleInput(SDL_Event& event, std::vector<Enemy>& enemies, Character& player, std::vector<Item>& items);
    void Move(double delta_time, std::vector<std::vector<int> >& map_data);
    
    void ShowPosition(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* camera);
    bool CheckCollision(int x, int y, std::vector<std::vector<int> >& map_data);
    
    void TakeDamage(int damage);
    void Attack(std::vector<Enemy>& enemies, Character& player, std::vector<Item>& items);
    SDL_Rect GetAttackBox();
    int GetHP() const { return hp_; } 
    int GetMP() const { return mp_; } 

    void SetQuest(Quest* q);
    void DoQuest();
    void QuestReward();
    void NextQuest();
    int GetQuestState() {return quest_stage_;}
    std::string GetCurrentQuestInfo();
    Quest* GetCurrentQuest() {return active_quest_;}

    bool LoadImg(SDL_Renderer* renderer);
    void Render(SDL_Renderer* des, SDL_Rect* camera);
    SDL_Rect GetRect() const { return player_.GetRect(); }
    SDL_Rect GetPosition() const { return {int(pos_x_) , int (pos_y_), 21, 38};}

    void UpdateState(int new_state);
    void UpdateAnimation();

    

private:
    Graphic player_;
    double pos_x_, pos_y_;
    double vel_x_, vel_y_;
    double gravity_y;
    double gravity_x;
    double jump;
    const double SPEED = 150.0;
    double target_speed_x_ = 0.0;
    int h_player = 38;
    int w_player = 21;
    bool on_ground_;
    bool flag_right_;
    bool flag_left_;
    bool flag_up_;
    bool is_right_;
    int hp_, max_hp_; 
    int mp_, max_mp_;
    
    bool is_attacking_;
    Uint32 last_attack_time_;
    Uint32 attack_cooldown_; 
    int attack_damage_;
    int attack_range_;

    Quest* active_quest_;
    int quest_stage_;

    int state;
    int frame;
    int frame_count;
    int frame_width;
    int frame_height;
    Uint32 last_frame_time;
    int frame_delay;

    Graphic run_right_texture;
    Graphic run_left_texture;
    Graphic jump_right_texture;
    Graphic jump_left_texture;
    Graphic stand_right_texture;
    Graphic stand_left_texture;
};

#endif
