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
    
    void ShowPotion(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* camera);
    bool CheckCollision(int x, int y, std::vector<std::vector<int> >& map_data);
    
    void TakeDamage(int damage);
    void Attack(std::vector<Enemy>& enemies, Character& player, std::vector<Item>& items);
    SDL_Rect GetAttackBox();
    int GetHP() const { return hp_; } 
    int GetMP() const { return mp_; } 

    void SetQuest(Quest* q);
    void DoQuest(int id);
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
    void UpdateAttackAnimation();
    void UpdateRegeneration(Uint32 current_time);

    void PickUpItem(int id);
    void UseItemHp();
    void UseItemMp();
    void SetItemHp(int quantity) {item_hp = quantity;}
    void SetItemMp(int quantity) {item_mp = quantity;}
    int GetItemHp() const {return item_hp;}
    int GetItemMp() const {return item_mp;}
    int GetState() const {return state;}
    
    void RenderHPMPBars(SDL_Renderer* screen);
    void Reset();
    
private:
    Graphic player_;
    double pos_x_, pos_y_;
    double vel_x_, vel_y_;
    double gravity_y, gravity_x;
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

    int frame_attack;
    int frame_count_attack;
    int frame_width_attack;
    int frame_height_attack;
    Uint32 last_frame_attack_time;

    Graphic run_right_texture;
    Graphic run_left_texture;
    Graphic jump_right_texture;
    Graphic jump_left_texture;
    Graphic stand_right_texture;
    Graphic stand_left_texture;
    Graphic attack_right_texture;
    Graphic attack_left_texture;

    int item_hp;
    int item_mp;

    Uint32 idle_start_time_;    // Thời gian bắt đầu đứng im
    Uint32 last_regen_time_;    // Thời gian hồi phục cuối cùng
    bool is_idle_;              // Trạng thái đứng im
    const int REGEN_DELAY = 5000; // 5 giây (5000ms) để bắt đầu hồi phục
    const int REGEN_RATE = 500;   // 1 giây (1000ms) để hồi phục 2 lần
    const int HP_REGEN = 10;        // Số HP hồi mỗi giây
    const int MP_REGEN = 10;        // Số MP hồi mỗi giây

    // Biến mới cho thanh HP và MP
    int hp_bar_width_;    // Chiều rộng thanh HP
    int hp_bar_height_;   // Chiều cao thanh HP
    int mp_bar_width_;    // Chiều rộng thanh MP
    int mp_bar_height_;   // Chiều cao thanh MP
    SDL_Color hp_color_;  // Màu thanh HP (xanh lá)
    SDL_Color mp_color_;  // Màu thanh MP (xanh dương)
    SDL_Color bg_color_;  // Màu nền (đỏ)
};

#endif
