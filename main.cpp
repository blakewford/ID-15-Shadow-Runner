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
    if(!inRange(x, y)) return;

    int32_t index = (y*image.width)+x;
    image.image[index] = value;
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

void convertImage(const unsigned char* bitmap, const uint8_t width, const uint8_t height, pgm& converted)
{
    converted.width  = width;
    converted.height = height;
    converted.image = new float[converted.width*converted.height];

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

void convertImageAndMask(const unsigned char* bitmap, const uint8_t width, const uint8_t height, pgm& converted, pgm& mask)
{
    converted.width  = width;
    converted.height = height;
    converted.image = new float[converted.width*converted.height];

    mask.width  = width;
    mask.height = height;
    mask.image = new float[mask.width*mask.height];

    uint16_t i = 2; //offset past dimensions

    uint16_t j = 0;
    uint16_t k = 0;
    uint16_t pixel = 0;
    while(j < converted.height)
    {
        while(k < (converted.width*2))
        {
            converted.image[pixel] = (bitmap[i+k] & 1<<(j%8)) ? 255.0f: 0.0f;
            k++;
            mask.image[pixel++] = (bitmap[i+k] & 1<<(j%8)) ? 255.0f: 0.0f;
            k++;
        }
        k = 0;
        j++;
        if(j%8 == 0)
        {
            i+=(converted.width*2);
        }
    }
}

bool clipImage(pgm& modified, int16_t x, int16_t y)
{
    if((x >= WIDTH) || (y >= HEIGHT))
    {
        return false;
    }

    bool canClip = false;

    int16_t visibleX = 0;
    int16_t visibleY = 0;
    if(x < 0)
    {
        visibleX = x+modified.width;
        if(visibleX < 0) return false;

        modified.width = visibleX;
        canClip = true;
    }

    if(y < 0)
    {
        visibleY = y+modified.height;
        if(visibleY < 0)
        {
            return false;
        }

        modified.height = visibleY;
        canClip = true;
    }

    return canClip;
}

void writeToScreen(const pgm& image, int16_t x, int16_t y)
{
    int16_t i = 0;
    int16_t j = 0;
    int32_t pixel = 0;

    pgm modified = image;
    if(!inRange(x, y))
    {
        if(!clipImage(modified, x, y)) return;
    }

    while(j < modified.height)
    {
        while(i < modified.width)
        {
            pixel = getPixel(modified, i, j);
            if(pixel != 0.0f)
            {
                setPixel(gScreen, x+i, y+j, pixel);
            }
            i++;
        }
        i=0;
        j++;
    }
}

void eraseFromScreen(const pgm& image, int16_t x, int16_t y)
{
    if(!inRange(x, y)) return;

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

int32_t calculateOffset(const unsigned char* bitmap, const uint8_t frame)
{
    return ((bitmap[0]*bitmap[1])*frame)/8;
}

void writeToScreen(const unsigned char* bitmap, int16_t x, int16_t y, uint8_t frame)
{
    pgm item;
    int32_t offset = calculateOffset(bitmap, frame);
    convertImage(bitmap+offset, bitmap[0], bitmap[1], item);
    item.height = bitmap[1];
    writeToScreen(item, x, y);

    delete[] item.image;
    item.image = nullptr;
}

void maskToScreen(const unsigned char* bitmap, int16_t x, int16_t y, uint8_t frame)
{
    pgm item;
    pgm mask;
    int32_t offset = (((bitmap[0]*bitmap[1])*frame)*2)/8;
    convertImageAndMask(bitmap+offset, bitmap[0], bitmap[1], item, mask);

    delete[] item.image;
    item.image = nullptr;

    delete[] mask.image;
    mask.image = nullptr;
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
    std::this_thread::sleep_for(milliseconds(ms));
}

long random(long howsmall, long howbig)
{
    long diff = howbig - howsmall;
    return howsmall + (rand()%diff);
}

char* ltoa(long l, char * buffer, int radix)
{
    if(radix != 10) assert(0);

    sprintf(buffer, "%ld", l);
    return buffer;
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
    return false;
}

bool Arduboy2Base::collide(Rect rect1, Rect rect2)
{
  return !(rect2.x >= rect1.x + rect1.width || rect2.x + rect2.width <= rect1.x || rect2.y >= rect1.y + rect1.height || rect2.y + rect2.height <= rect1.y);
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
    else if(bitmap == forgroundTrees)
    {
        size = sizeof(forgroundTrees);
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
    {
        writeToScreen(bitmap, x, y, frame);
    }
}

void Sprites::drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    unsigned long int size = getImageSize(bitmap);
    if(size != 0)
    {
        pgm mask;
        int32_t offset = calculateOffset(bitmap, frame);
        convertImage(bitmap+offset, bitmap[0], bitmap[1], mask);
        mask.height = bitmap[1];
        eraseFromScreen(mask, x, y);
        delete[] mask.image;
        mask.image = nullptr;
    }
}

void Sprites::drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame)
{
    unsigned long int size = getImageSize(bitmap)/2;
    if(size != 0)
    {
        maskToScreen(bitmap, x, y, frame);
    }
}
