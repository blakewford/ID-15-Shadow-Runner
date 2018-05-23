#pragma once

#include "Sprites.h"
#include "Arduboy2Audio.h"

class Arduboy2Base
{
public:
    void begin();
    void setFrameRate(uint8_t rate);
    void initRandomSeed();
    bool everyXFrames(uint8_t frames);
    bool justPressed(uint8_t button);

    Arduboy2Audio audio;
};
