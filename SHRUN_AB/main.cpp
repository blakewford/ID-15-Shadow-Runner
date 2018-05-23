#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "SHRUN_AB.ino"

void writePgm(const unsigned char* bitmap, unsigned long int size)
{
    const uint16_t width = bitmap[0];
    const uint16_t height = bitmap[1];
    int8_t frames = size/((width*height)/8); //eight pixels per byte

    printf("P2\n");
    printf("%d %d\n", width, height*frames);
    printf("255\n");

    uint16_t i = 2; //offset past dimensions

    while(frames--)
    {
        uint16_t j = 0;
        uint16_t k = 0;
        while(j < height)
        {
            while(k < width)
            {
                printf("%d\n", (bitmap[i+k] & 1<<(j%8)) ? 255: 0);
                k++;
            }
            k = 0;
            j++;
            if(j%8 == 0)
            {
                i+=width;
            }
        }
    }
}

int main()
{
//    writePgm(T_arg, sizeof(T_arg));
    setup();
    while(true)
        loop();
}

void delay(uint32_t ms)
{
    assert(0);
}

long random(long howsmall, long howbig)
{
    assert(0);
    return 0;
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
    assert(0);
    return true;
}

bool Arduboy2Base::justPressed(uint8_t button)
{
    assert(0);
}

ArduboyTones::ArduboyTones(bool (*outEn)())
{
}

bool Arduboy2Audio::enabled()
{
    assert(0);
    return false;
}

void Arduboy2Audio::off()
{
    assert(0);
}

void Arduboy2Audio::on()
{
    assert(0);
}

void Arduboy2Audio::saveOnOff()
{
    assert(0);
}

void Sprites::drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    assert(0);
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    assert(0);
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    assert(0);
}