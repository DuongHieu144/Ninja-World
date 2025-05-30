#include "commonFunc.h"
#include "character.h"
#include "map.h"
#include "enemy.h"

Character::Character()
{
    pos_x_ = 90;
    pos_y_ = 418;
    vel_x_ = 0;
    vel_y_ = 0;
    gravity_y = 1000.0;
    gravity_x = 600.0;
    jump = -330.0;
    on_ground_ = false;
    flag_right_ = false;
    flag_left_ = false;
    flag_up_ = false;
    is_right_ = false;
    player_.SetRect((int)pos_x_, (int)pos_y_);
    hp_ = max_hp_ = 500;
    mp_ = max_mp_ = 200;  
    is_attacking_ = false;
    attack_damage_ = 20;
    attack_range_ = 40;
    last_attack_time_ = 0;
    attack_cooldown_ = 500; 

    quest_stage_ = 0;
    active_quest_ = nullptr;

    state = 0;
    frame = 0;
    frame_width = 21;  
    frame_height = 38;
    frame_delay = 100;
    last_frame_time = SDL_GetTicks();

    frame_attack = 0;
    frame_count_attack = 4;
    frame_width_attack = 48;
    frame_height_attack = 38;
    last_frame_attack_time = SDL_GetTicks();

    item_hp = 0;
    item_mp = 0;

    idle_start_time_ = SDL_GetTicks();
    last_regen_time_ = SDL_GetTicks();
    is_idle_ = true;

    hp_bar_width_ = 150;       
    hp_bar_height_ = 10;       
    mp_bar_width_ = 150;      
    mp_bar_height_ = 10;       
    hp_color_ = {0, 255, 0, 255}; 
    mp_color_ = {0, 0, 255, 255}; 
    bg_color_ = {255, 0, 0, 255}; 
}

void Character::SetQuest(Quest* q)
{
    active_quest_ = q;
}

void Character::DoQuest(int id)
{
    if(id == quest_stage_+1)
        if(active_quest_)
            active_quest_->InCurrent();
}

void Character::QuestReward()
{
    int type = active_quest_->GetRewardType();
    if(type == 1) 
    {
        attack_cooldown_ = 300;
        attack_damage_ +=10;
        max_hp_ += 200;
        max_mp_ += 100;
    }
    else if(type == 2) 
    {
        jump = -400;
        max_hp_ += 200;
        max_mp_ += 50;
        attack_damage_ +=20;
    }
    else if(type == 3)
    {
        max_hp_ += 200;
        max_mp_ += 50;
        attack_damage_ +=20;
    }
    else if(type == 4)
    {
        max_hp_ += 200;
        max_mp_ += 10;
        attack_damage_ +=20;
    }
    else if(type == 5)
    {
        max_hp_ += 200;
        max_mp_ += 10;
        attack_damage_ +=20;
    }

}

void Character::NextQuest()
{
    quest_stage_ ++;
    active_quest_ = nullptr;
}

std::string Character::GetCurrentQuestInfo()
{
    if(active_quest_)
    {
        if(active_quest_->IsCompleted()) return "Quest: Meet the village chief to complete the quest";
        return "Quest: " + active_quest_->GetDescription() + 
        " (" + std::to_string(active_quest_->GetProgress()) +
        "/" + std::to_string(active_quest_->GetGoal()) + ")";
    }
    return "Receive a quest from the village chief";
}

void Character::TakeDamage(int damage)
{
    hp_ -= damage;
    if (hp_ < 0) hp_ = 0;
}

Character::~Character()
{
    player_.Free();
}

bool Character::LoadImg(SDL_Renderer* renderer)
{
    bool tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
    tmp1 = run_right_texture.LoadImg("img/run_right.png", renderer);
    tmp2 = run_left_texture.LoadImg("img/run_left.png", renderer);
    tmp3 = jump_right_texture.LoadImg("img/jump_right.png", renderer);
    tmp4 = jump_left_texture.LoadImg("img/jump_left.png", renderer);
    tmp5 = stand_right_texture.LoadImg("img/stand_right.png", renderer);
    tmp6 = stand_left_texture.LoadImg("img/stand_left.png", renderer);
    tmp7 = attack_right_texture.LoadImg("img/attack_right.png", renderer);
    tmp8 = attack_left_texture.LoadImg("img/attack_left.png", renderer);
    return (tmp1 & tmp2 & tmp3 & tmp4 & tmp5 & tmp6 & tmp7 & tmp8);
}

bool Character::CheckCollision(int x, int y, std::vector<std::vector<int> >& map_data)
{
    int left_tile = x / TILE_SIZE_WIDTH;
    int right_tile = (x + w_player - 1) / TILE_SIZE_WIDTH;
    int top_tile = y / TILE_SIZE_HEIGHT;
    int bottom_tile = (y + h_player - 1) / TILE_SIZE_HEIGHT;

    for (int i = top_tile; i <= bottom_tile; ++i) 
    {
        for (int j = left_tile; j <= right_tile; ++j) 
        {
            if (map_data[i][j] == 0) 
            {
                continue;
            }
            if (map_data[i][j] == 2)
            {
                if (vel_y_ > 0)
                {
                    int platform_y = i * TILE_SIZE_HEIGHT;
                    if (pos_y_ + h_player <= platform_y + 5)
                    {
                        return true;
                    }
                }
                continue; 
            }
            if(map_data[i][j] == 1) return true;

        }
    }
    if (x < 0 || x + w_player > map_width || y < 0 || y + h_player > map_height)
    {
        if (y + h_player > map_height)
            on_ground_ = true;
        return true;
    }

    return false;
}

SDL_Rect Character::GetAttackBox()
{
    SDL_Rect attack_rect;

    if (is_right_) 
    {
        attack_rect = { int(pos_x_) , int (pos_y_), attack_range_ + 21, 38 };
    } 
    else 
    {
        attack_rect = { int (pos_x_) - attack_range_, int (pos_y_), attack_range_ + 21, 38 };
    }

    return attack_rect;
}

void Character::Attack(std::vector<Enemy>& enemies, Character& player, std::vector<Item>& item_list)
{
    Uint32 now = SDL_GetTicks();
    if (now - last_attack_time_ < attack_cooldown_) return; 

    last_attack_time_ = now;

    SDL_Rect attack_box = GetAttackBox();

    for (Enemy& enemy : enemies)
    {
        if (!enemy.IsDead())
        {
            SDL_Rect enemy_box = enemy.GetHitBox();
            if (SDL_HasIntersection(&attack_box, &enemy_box))
            {
                if(mp_>=5)
                {
                    is_attacking_ = true;
                    frame_attack = 0;
                    enemy.TakeDamage(attack_damage_, player, item_list);
                    mp_-=5;
                    Mix_PlayChannel(-1, g_attack_sound, 0);
                }
            }
        }
    }
}

void Character::HandleInput(SDL_Event& event, std::vector<Enemy>& enemies, Character& player, std::vector<Item>& items)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                flag_up_ = true;
                if (on_ground_ && flag_up_)
                {
                    vel_y_ = jump;
                    on_ground_ = false;
                    UpdateState(2);
                }
                
                break;
            case SDLK_LEFT: target_speed_x_ = -SPEED;
                            vel_x_= target_speed_x_; 
                            flag_left_ = true;
                            is_right_ = false;
                            if(!on_ground_) UpdateState(2);
                            else UpdateState(1);
                            break;
            case SDLK_RIGHT: target_speed_x_ = SPEED;
                            vel_x_= target_speed_x_;
                            flag_right_ = true;
                            is_right_ = true;
                            if(!on_ground_) UpdateState(2);
                            else UpdateState(1);
                            break;
            case SDLK_RETURN: Attack(enemies, player, items);
                            break;
            case SDLK_1: UseItemHp();
                        break;
            case SDLK_2: UseItemMp();
                        break;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT: flag_left_ = false;
                            if(!flag_right_) 
                            {
                                target_speed_x_ = 0;
                                UpdateState(0);
                                if(!on_ground_) UpdateState(2);
                            }
                            else 
                            {
                                target_speed_x_ = SPEED;
                                vel_x_ = target_speed_x_;
                                is_right_ = true;
                                if(flag_up_) UpdateState(2);
                                else UpdateState(1);
                            }
                            break;

            case SDLK_RIGHT: flag_right_ = false;
                            if(!flag_left_) 
                            {
                                target_speed_x_ = 0;
                                UpdateState(0);
                                if(!on_ground_) UpdateState(2);
                            }
                            else 
                            {
                                target_speed_x_ = -SPEED;
                                vel_x_ = target_speed_x_;
                                is_right_ = false;
                                if(flag_up_) UpdateState(2);
                                else UpdateState(1);
                            }
                            break;
            case SDLK_UP: if(!on_ground_) UpdateState(2);
                            else UpdateState(0);
                            flag_up_ = false;
                            break;
                            
        }
    }
}

void Character::Move(double delta_time, std::vector<std::vector<int> >& map_data)
{
    bool was_on_ground = on_ground_;
    if(on_ground_) 
    {
        if (vel_x_ < target_speed_x_)
        {
            vel_x_ += gravity_x * delta_time;
            if(vel_x_ > target_speed_x_) vel_x_ = target_speed_x_;
        }
        else if (vel_x_ > target_speed_x_)
        {
            vel_x_ -= gravity_x * delta_time;
            if(vel_x_ < target_speed_x_) vel_x_ = target_speed_x_;
        }
    }
    double new_pos_x = pos_x_ + vel_x_ * delta_time;
    if (!CheckCollision((int)new_pos_x, (int)pos_y_, map_data))
    {
        pos_x_ = new_pos_x;
    }
    else
    {
        if(vel_x_ > 0)
        {
            vel_x_ = 0;
        }
    }

    vel_y_ += gravity_y * delta_time;
    double new_pos_y = pos_y_ + vel_y_ * delta_time;
    if (!CheckCollision((int)pos_x_, (int)new_pos_y, map_data))
    {
        pos_y_ = new_pos_y;
        on_ground_ = false;
    }
    else
    {
        if (vel_y_ > 0)
        {
            vel_y_ = 0;
            on_ground_ = true;
           if(on_ground_ && !was_on_ground) UpdateState(0);
           if(flag_left_ || flag_right_) UpdateState(1);
        }
    }
    if (!was_on_ground && on_ground_) 
    {
        if(!flag_left_ && !flag_right_)
        {
            vel_x_ = 0;           
            target_speed_x_ = 0; 
        }   
        else
        {
            if(flag_left_) target_speed_x_ = -SPEED;
            else target_speed_x_ = SPEED;
        }
    }
    
    player_.SetRect((int)pos_x_, (int)pos_y_);

    UpdateRegeneration(SDL_GetTicks());
}

void Character::ShowPotion(SDL_Renderer* renderer, TTF_Font* font, SDL_Rect* camera) 
{
    std::string text = "HP Potion: " + std::to_string(item_hp) + "  MP Potion: " + std::to_string(item_mp);

    SDL_Color textColor = {255, 255, 255}; 

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (textSurface != nullptr) 
    {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textW = textSurface->w;
        int textH = textSurface->h;

        SDL_Rect renderQuad = {SCREEN_WIDTH - textW - 10, 10, textW, textH};  

        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void Character::UpdateState(int new_state)
{
    if(state == new_state) return;
    state = new_state;
    frame=0;
    switch (state) 
    {
        case 0:
            frame_width = 21;
            frame_height = 38;
            frame_count = 1; 
            break;
        case 1:
            frame_width = 21;
            frame_height = 38;
            frame_count = 6; 
            break;
        case 2:
            frame_width = 26;
            frame_height = 38;
            frame_count = 6; 
            break;
    }
}

void Character::UpdateAnimation()
{
    Uint32 current_time = SDL_GetTicks();
    if(current_time > last_frame_time + 100)
    {
        frame ++;
        if(frame == frame_count) frame = 0;
        last_frame_time = current_time;
    }
}
void Character::UpdateAttackAnimation()
{
    Uint32 current_time = SDL_GetTicks();
    if(current_time > last_frame_attack_time + 50)
    {
        frame_attack ++;
        if(frame_attack == frame_count_attack) 
        {
            frame_attack = 0;
            is_attacking_ = false;
        }
        last_frame_attack_time = current_time;
    }
}

void Character::PickUpItem(int id)
{
    if(id == 1) item_hp++;
    if(id == 2) item_mp++;
}

void Character::UseItemHp()
{
    if(item_hp > 0)
    {
        item_hp--;
        if(hp_ + 200 >= max_hp_) hp_=max_hp_;
        else hp_+=200;
    }
}

void Character::UseItemMp()
{
    if(item_mp > 0)
    {
        item_mp--;
        if(mp_ + 200 >= max_mp_) mp_=max_mp_;
        else mp_+=200;
    }
}

void Character::UpdateRegeneration(Uint32 current_time) {
    // Kiểm tra trạng thái đứng im
    if (vel_x_ == 0 && vel_y_ == 0 && !is_attacking_ && !flag_left_ && !flag_right_ && !flag_up_) {
        if (!is_idle_) {
            // Bắt đầu đứng im
            is_idle_ = true;
            idle_start_time_ = current_time;
        }
    } else {
        // Không đứng im nữa, reset thời gian
        is_idle_ = false;
        idle_start_time_ = current_time;
        return;
    }

    // Nếu đứng im đủ 10 giây, bắt đầu hồi phục
    if (is_idle_ && (current_time - idle_start_time_ >= REGEN_DELAY)) {
        if (current_time - last_regen_time_ >= REGEN_RATE) {
            // Hồi phục HP
            if (hp_ < max_hp_) {
                hp_ += HP_REGEN;
                if (hp_ > max_hp_) hp_ = max_hp_;
            }
            // Hồi phục MP
            if (mp_ < max_mp_) {
                mp_ += MP_REGEN;
                if (mp_ > max_mp_) mp_ = max_mp_;
            }
            last_regen_time_ = current_time; // Cập nhật thời gian hồi phục cuối
        }
    }
}

void Character::RenderHPMPBars(SDL_Renderer* screen) {
    const int OFFSET_X = 50; // Khoảng cách từ lề trái để đặt hình nhân vật
    const int OFFSET_Y = 10; // Khoảng cách từ lề trên
    const int BAR_SPACING = 5; // Khoảng cách giữa thanh HP và MP

    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface* hpTextSurface = TTF_RenderText_Solid(g_font, "HP", textColor);
    if (hpTextSurface) 
    {
        SDL_Texture* hpTextTexture = SDL_CreateTextureFromSurface(screen, hpTextSurface);
        int textW = hpTextSurface->w;
        SDL_Rect hpTextRect = {13, 3, textW, hpTextSurface->h};
        SDL_RenderCopy(screen, hpTextTexture, nullptr, &hpTextRect);
        SDL_FreeSurface(hpTextSurface);
        SDL_DestroyTexture(hpTextTexture);
    }

    SDL_Surface* mpTextSurface = TTF_RenderText_Solid(g_font, "MP", textColor);
    if (mpTextSurface) {
        SDL_Texture* mpTextTexture = SDL_CreateTextureFromSurface(screen, mpTextSurface);
        int textW = mpTextSurface->w;
        SDL_Rect mpTextRect = {13, 3 + hp_bar_height_ + BAR_SPACING, textW, mpTextSurface->h};
        SDL_RenderCopy(screen, mpTextTexture, nullptr, &mpTextRect);
        SDL_FreeSurface(mpTextSurface);
        SDL_DestroyTexture(mpTextTexture);
    }

    // Vẽ nền thanh HP (màu đỏ)
    SDL_Rect hp_bg_rect = { OFFSET_X, OFFSET_Y, hp_bar_width_, hp_bar_height_ };
    SDL_SetRenderDrawColor(screen, bg_color_.r, bg_color_.g, bg_color_.b, bg_color_.a);
    SDL_RenderFillRect(screen, &hp_bg_rect);

    // Vẽ thanh HP hiện tại (màu xanh lá)
    float hp_ratio = (float)hp_ / max_hp_;
    int current_hp_width = (int)(hp_bar_width_ * hp_ratio);
    SDL_Rect hp_rect = { OFFSET_X, OFFSET_Y, current_hp_width, hp_bar_height_ };
    SDL_SetRenderDrawColor(screen, hp_color_.r, hp_color_.g, hp_color_.b, hp_color_.a);
    SDL_RenderFillRect(screen, &hp_rect);

    // Vẽ nền thanh MP (màu đỏ)
    SDL_Rect mp_bg_rect = { OFFSET_X, OFFSET_Y + hp_bar_height_ + BAR_SPACING, mp_bar_width_, mp_bar_height_ };
    SDL_SetRenderDrawColor(screen, bg_color_.r, bg_color_.g, bg_color_.b, bg_color_.a);
    SDL_RenderFillRect(screen, &mp_bg_rect);

    // Vẽ thanh MP hiện tại (màu xanh dương)
    float mp_ratio = (float)mp_ / max_mp_;
    int current_mp_width = (int)(mp_bar_width_ * mp_ratio);
    SDL_Rect mp_rect = { OFFSET_X, OFFSET_Y + hp_bar_height_ + BAR_SPACING, current_mp_width, mp_bar_height_ };
    SDL_SetRenderDrawColor(screen, mp_color_.r, mp_color_.g, mp_color_.b, mp_color_.a);
    SDL_RenderFillRect(screen, &mp_rect);
}

void Character::Reset()
{
    // Giải phóng các texture cũ
    run_right_texture.Free();
    run_left_texture.Free();
    jump_right_texture.Free();
    jump_left_texture.Free();
    stand_right_texture.Free();
    stand_left_texture.Free();
    attack_right_texture.Free();
    attack_left_texture.Free();
    player_.Free();

    // Đặt lại các giá trị ban đầu
    pos_x_ = 90;
    pos_y_ = 418;
    vel_x_ = 0;
    vel_y_ = 0;
    gravity_y = 1000.0;
    gravity_x = 600.0;
    jump = -330.0;
    on_ground_ = false;
    flag_right_ = false;
    flag_left_ = false;
    flag_up_ = false;
    is_right_ = false;
    player_.SetRect((int)pos_x_, (int)pos_y_);
    hp_ = max_hp_ = 500;
    mp_ = max_mp_ = 200;  
    is_attacking_ = false;
    attack_damage_ = 20;
    attack_range_ = 40;
    last_attack_time_ = 0;
    attack_cooldown_ = 500; 

    quest_stage_ = 0;
    active_quest_ = nullptr;

    state = 0;
    frame = 0;
    frame_width = 21;  
    frame_height = 38;
    frame_delay = 100;
    last_frame_time = SDL_GetTicks();

    frame_attack = 0;
    frame_count_attack = 4;
    frame_width_attack = 48;
    frame_height_attack = 38;
    last_frame_attack_time = SDL_GetTicks();

    item_hp = 0;
    item_mp = 0;

    idle_start_time_ = SDL_GetTicks();
    last_regen_time_ = SDL_GetTicks();
    is_idle_ = true;

    hp_bar_width_ = 150;       
    hp_bar_height_ = 10;       
    mp_bar_width_ = 150;      
    mp_bar_height_ = 10;       
    hp_color_ = {0, 255, 0, 255}; 
    mp_color_ = {0, 0, 255, 255}; 
    bg_color_ = {255, 0, 0, 255}; 
}

void Character::Render(SDL_Renderer* des, SDL_Rect* camera)
{
   
    SDL_Rect src_rect = { frame * frame_width, 0, frame_width, frame_height };
    SDL_Rect dst_rect = { (int)pos_x_ - camera->x, (int)pos_y_ - camera->y, frame_width, frame_height };
    if(is_attacking_) 
    {
        SDL_Rect src_rect_attack = { frame_attack * 48, 0, 48, 38 };
        SDL_Rect dst_rect_attack = { (int)pos_x_ - camera->x, (int)pos_y_ - camera->y, 48, 38 };
        if(is_right_) 
        {
            SDL_Rect dst_rect_attack = { (int)pos_x_ - camera->x, (int)pos_y_ - camera->y, 48, 38 };
            attack_right_texture.Render(des, &src_rect_attack, &dst_rect_attack);
        }
        else 
        {
            SDL_Rect dst_rect_attack = { (int)pos_x_ - camera->x - 27, (int)pos_y_ - camera->y, 48, 38 };
            attack_left_texture.Render(des, &src_rect_attack, &dst_rect_attack);
        }
        if(frame == 3) is_attacking_ = false;
    }
    else
    if (!is_right_) {
        if (state == 0) {
            stand_left_texture.Render(des, &src_rect, &dst_rect);
        } 
        else if (state == 1) {
            run_left_texture.Render(des, &src_rect, &dst_rect);
        } 
        else if (state == 2) {
            jump_left_texture.Render(des, &src_rect, &dst_rect);
        }
    } 
    else {
        if (state == 0) {
            stand_right_texture.Render(des, &src_rect, &dst_rect);
        } 
        else if (state == 1) {
            run_right_texture.Render(des, &src_rect, &dst_rect);
        } 
        else if (state == 2) {
            jump_right_texture.Render(des, &src_rect, &dst_rect);
        }
    }
    RenderHPMPBars(des);

}

