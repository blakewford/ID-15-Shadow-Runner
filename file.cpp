#include <chrono>
#include <stdio.h>
#include <stdint.h>
#include <thread>

#include "SHRUN_AB.ino"


bool gKeepGoing = true;

uint64_t gFrame = 0;

void delay(uint32_t ms)
{
}

long random(long howsmall, long howbig)
{
    return howsmall;
}

char* ltoa_compat(long l, char * buffer, int radix)
{
    buffer[0] = '0';
    return buffer;
}

void Arduboy2Base::begin()
{
}

void Arduboy2Base::setFrameRate(uint8_t rate)
{
}

void Arduboy2Base::initRandomSeed()
{
}

bool Arduboy2Base::everyXFrames(uint8_t frames)
{
    return true;
}

bool Arduboy2Base::justPressed(uint8_t button)
{
    bool pressed = false;
    return pressed;
}

bool Arduboy2Base::collide(Rect rect1, Rect rect2)
{
  return !(rect2.x >= rect1.x + rect1.width || rect2.x + rect2.width <= rect1.x || rect2.y >= rect1.y + rect1.height || rect2.y + rect2.height <= rect1.y);
}

bool Arduboy2Base::nextFrame()
{
    gFrame++;
    return true;
}

void Arduboy2Base::pollButtons()
{
}

void Arduboy2Base::clear()
{
}

void Arduboy2Base::display()
{
}

ArduboyTones::ArduboyTones(bool (*outEn)())
{
}

void ArduboyTones::tone(uint16_t freq, uint16_t dur)
{
}

bool gAudioEnabled = true;

bool Arduboy2Audio::enabled()
{
    return gAudioEnabled;
}

void Arduboy2Audio::off()
{
    gAudioEnabled = false;
}

void Arduboy2Audio::on()
{
    gAudioEnabled = true;
}

void Arduboy2Audio::saveOnOff()
{
}

void Sprites::drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
}

int main()
{
    arduboy.clear();

    setup();
    while(gKeepGoing)
    {
        loop();
    }
}

#if defined(__mips__)
extern "C" void __start()
{
    main();
}
#endif

#if defined(__arm__) || defined (__powerpc__)
extern "C" void _start()
{
    main();
}
#endif
