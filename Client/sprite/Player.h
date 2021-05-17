#pragma once
#include "../engine/SpriteEngine.h"
class Player :
    public SpriteEngine::Sprite
{
private:
    uint8_t current_anim_frame;
    SDL_Texture* halo;
public:
    Player();

    void render() override;
};

