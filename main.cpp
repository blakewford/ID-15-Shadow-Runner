#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <fstream>

#include <thread>
#include <chrono>
using namespace std::chrono;

#include "SHRUN_AB.ino"

float SCREEN_DATA[WIDTH*HEIGHT];

struct pgm
{
    const char* magic = "P2";
    const int32_t max = 255;
    int32_t width, height;
    float* image;

    pgm()
    {
        width = 0;
        height = 0;
        image = nullptr;
    }
};

pgm gScreen;
uint64_t gFrame = 0;
system_clock::time_point syncPoint;
milliseconds gFrameRate = milliseconds(1000);

bool inRange(int32_t x, int32_t y)
{
    return (x >= 0 && x < WIDTH) && (y >= 0 && y < HEIGHT);
}

float getPixel(const pgm& image, int32_t x, int32_t y)
{
    int32_t index = (y*image.width)+x;
    return image.image[index];
}

void setPixel(const pgm& image, int32_t x, int32_t y, float value)
{
    if(inRange(x, y))
    {
        int32_t index = (y*image.width)+x;
        image.image[index] = value;
    }
}

void writeImage(const pgm& image, const char* file)
{
    std::ofstream stream;
    stream.open(file);
    stream << image.magic << "\n" << image.width << " " << image.height << "\n" << image.max << "\n";

    int32_t j = 0;
    int32_t i = (image.height*image.width);
    while(i--)
    {
         stream << image.image[j] << "\n";
         j++;
    }

    stream.close();
}

void convertImage(const unsigned char* bitmap, const uint8_t width, const uint8_t height, const unsigned long int size, pgm& converted)
{
    converted.width  = width;
    converted.height = height;
    converted.image = new float[converted.width*converted.height];

    int8_t frames = size/((converted.width*converted.height)/8); //eight pixels per byte

    uint16_t i = 2; //offset past dimensions

    uint16_t j = 0;
    uint16_t k = 0;
    uint16_t pixel = 0;
    while(j < converted.height)
    {
        while(k < converted.width)
        {
            converted.image[pixel++] = (bitmap[i+k] & 1<<(j%8)) ? 255.0f: 0.0f;
            k++;
        }
        k = 0;
        j++;
        if(j%8 == 0)
        {
            i+=converted.width;
        }
    }
}

void writeToScreen(const pgm& image, int16_t x, int16_t y)
{
    if(inRange(x, y))
    {
        int16_t i = 0;
        int16_t j = 0;
        int32_t pixel = 0;
        while(j < image.height)
        {
            while(i < image.width)
            {
                pixel = getPixel(image, i, j);
                setPixel(gScreen, x+i, y+j, pixel);
                i++;
            }
            i=0;
            j++;
        }
    }
}

void eraseFromScreen(const pgm& image, int16_t x, int16_t y)
{
    if(inRange(x, y))
    {
        int16_t i = 0;
        int16_t j = 0;
        int32_t pixel = 0;
        while(j < image.height)
        {
            while(i < image.width)
            {
                pixel = getPixel(image, i, j);
                if(pixel != 0.0f)
                {
                    setPixel(gScreen, x+i, y+j, 0.0f);
                }
                i++;
            }
            i=0;
            j++;
        }
    }
}

void writeToScreen(const unsigned char* bitmap, const unsigned long int size, int16_t x, int16_t y, uint8_t frame)
{
    pgm item;
    int32_t offset = (bitmap[0]*bitmap[1])*frame;
    convertImage(bitmap+offset, bitmap[0], bitmap[1], size, item);
    item.height = bitmap[1];
    writeToScreen(item, x, y);

    delete[] item.image;
    item.image = nullptr;
}

int main()
{
    arduboy.clear();
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
    long diff = howbig - howsmall;
    return howsmall + (rand()%diff);
}

char* ltoa(long l, char * buffer, int radix)
{
    assert(0);
}

void Arduboy2Base::begin()
{
}

void Arduboy2Base::setFrameRate(uint8_t rate)
{
    gFrameRate = milliseconds(1000/rate);
}

void Arduboy2Base::initRandomSeed()
{
}

bool Arduboy2Base::everyXFrames(uint8_t frames)
{
    return gFrame%frames == 0;
}

bool Arduboy2Base::justPressed(uint8_t button)
{
}

bool Arduboy2Base::collide(Rect rect1, Rect rect2)
{
    assert(0);
    return false;
}

bool Arduboy2Base::nextFrame()
{
    while(system_clock::now() < syncPoint)
    {
//        std::this_thread::yield();
        std::this_thread::sleep_for(nanoseconds(1));
    }

    syncPoint = system_clock::now() + gFrameRate;
    gFrame++;
    return true;
}

void Arduboy2Base::pollButtons()
{
}

void Arduboy2Base::clear()
{
    gScreen.width = WIDTH;
    gScreen.height = HEIGHT;
    gScreen.image = SCREEN_DATA;
    memset(gScreen.image, 0, WIDTH*HEIGHT*sizeof(float));
}

void Arduboy2Base::display()
{
    writeImage(gScreen, "test.pgm");
}

ArduboyTones::ArduboyTones(bool (*outEn)())
{
}

void ArduboyTones::tone(uint16_t freq, uint16_t dur)
{
    assert(0);
}

bool Arduboy2Audio::enabled()
{
    return true;
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

unsigned long int getImageSize(const uint8_t *bitmap)
{
    unsigned long int size = 0;

    if(bitmap == T_arg)
    {
        size = sizeof(T_arg);
    }
    else if(bitmap == spotLight)
    {
        size = sizeof(spotLight);
    }
    else if(bitmap == menuTitle)
    {
        size = sizeof(menuTitle);
    }
    else if(bitmap == menuItems)
    {
        size = sizeof(menuItems);
    }
    else if(bitmap == menuYesNo)
    {
        size = sizeof(menuYesNo);
    }
    else if(bitmap == menuShade)
    {
        size = sizeof(menuShade);
    }
    else if(bitmap == menuInfo)
    {
        size = sizeof(menuInfo);
    }
    else if(bitmap == qrcode)
    {
        size = sizeof(qrcode);
    }
    else if(bitmap == pause)
    {
        size = sizeof(pause);
    }
    else if(bitmap == gameOver)
    {
        size = sizeof(gameOver);
    }
    else if(bitmap == life)
    {
        size = sizeof(life);
    }
    else if(bitmap == score)
    {
        size = sizeof(score);
    }
    else if(bitmap == lifeBar)
    {
        size = sizeof(lifeBar);
    }
    else if(bitmap == candleFlame)
    {
        size = sizeof(candleFlame);
    }
    else if(bitmap == candleTip)
    {
        size = sizeof(candleTip);
    }
    else if(bitmap == shadowRunner)
    {
        size = sizeof(shadowRunner);
    }
    else if(bitmap == shadowRunnerEyes)
    {
        size = sizeof(shadowRunnerEyes);
    }
    else if(bitmap == heart)
    {
        size = sizeof(heart);
    }
    else if(bitmap == stone_plus_mask)
    {
        size = sizeof(stone_plus_mask);
    }
    else if(bitmap == bird)
    {
        size = sizeof(bird);
    }
    else if(bitmap == numbers)
    {
        size = sizeof(numbers);
    }
    else if(bitmap == backGrounds)
    {
        size = sizeof(backGrounds);
    }
    else if(bitmap == fences_plus_mask)
    {
        size = sizeof(fences_plus_mask);
    }
    else
    {
        assert(0);
    }

    return size;
}

void Sprites::drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    unsigned long int size = getImageSize(bitmap);
    if(size != 0)
        writeToScreen(bitmap, size, x, y, frame);
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    unsigned long int size = getImageSize(bitmap);
    if(size != 0)
    {
        pgm mask;
        int32_t offset = (bitmap[0]*bitmap[1])*frame;
        convertImage(bitmap+offset, bitmap[0], bitmap[1], size, mask);
        mask.height = bitmap[1];
        eraseFromScreen(mask, x, y);
        delete[] mask.image;
        mask.image = nullptr;
    }
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    assert(0);
}
