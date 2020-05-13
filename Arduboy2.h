#pragma once

#include <stdint.h>
#include <string.h>

#define PROGMEM

typedef uint8_t byte;
typedef bool boolean;

const int16_t HEIGHT = 64;
const int16_t WIDTH  = 128;
const int32_t F_CPU = 16000000;

#define B00000000  0x0
#define B00000001  0x1
#define B00000010  0x2
#define B00000100  0x4
#define B00001000  0x8
#define B00001010  0xA
#define B00100000 0x20

void delay(uint32_t ms);
long random(long howsmall, long howbig);

enum
{
    LEFT_BUTTON,
    RIGHT_BUTTON,
    UP_BUTTON,
    DOWN_BUTTON,
    A_BUTTON,
    B_BUTTON,
};

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

char* ltoa_compat(long l, char * buffer, int radix);

#define pgm_read_word

/**
 * @file SpritesCommon.h
 * \brief
 * Common header file for sprite functions
 */

#ifndef SpritesCommon_h
#define SpritesCommon_h

#define SPRITE_MASKED 1
#define SPRITE_UNMASKED 2
#define SPRITE_OVERWRITE 2
#define SPRITE_PLUS_MASK 3
#define SPRITE_IS_MASK 250
#define SPRITE_IS_MASK_ERASE 251
#define SPRITE_AUTO_MODE 255

#endif

/**
 * @file Sprites.h
 * \brief
 * A class for drawing animated sprites from image and mask bitmaps.
 */

#ifndef Sprites_h
#define Sprites_h

/** \brief
 * A class for drawing animated sprites from image and mask bitmaps.
 *
 * \details
 * The functions in this class will draw to the screen buffer an image
 * contained in an array located in program memory. A mask can also be
 * specified or implied, which dictates how existing pixels in the buffer,
 * within the image boundaries, will be affected.
 *
 * A sprite or mask array contains one or more "frames". Each frame is intended
 * to show whatever the sprite represents in a different position, such as the
 * various poses for a running or jumping character. By specifying a different
 * frame each time the sprite is drawn, it can be animated.
 *
 * Each image array begins with values for the width and height of the sprite,
 * in pixels. The width can be any value. The height must be a multiple of
 * 8 pixels, but with proper masking, a sprite of any height can be created.
 *
 * For a separate mask array, as is used with `drawExternalMask()`, the width
 * and height are not included but must contain data of the same dimensions
 * as the corresponding image array.
 *
 * Following the width and height values for an image array, or from the
 * beginning of a separate mask array, the array contains the image and/or
 * mask data for each frame. Each byte represents a vertical column of 8 pixels
 * with the least significant bit (bit 0) at the top. The bytes are drawn as
 * 8 pixel high rows from left to right, top to bottom. When the end of a row
 * is reached, as specified by the width value, the next byte in the array will
 * be the start of the next row.
 *
 * Data for each frame after the first one immediately follows the previous
 * frame. Frame numbers start at 0.
 *
 * \note
 * \parblock
 * A separate `SpritesB` class is available as an alternative to this class.
 * The only difference is that the `SpritesB` class is optimized for small
 * code size rather than for execution speed. One or the other can be used
 * depending on whether size or speed is more important.
 *
 * Even if the speed is acceptable when using `SpritesB`, you should still try
 * using `Sprites`. In some cases `Sprites` will produce less code than
 * `SpritesB`, notably when only one of the functions is used.
 *
 * You can easily switch between using the `Sprites` class or the `SpritesB`
 * class by using one or the other to create an object instance:
 *
 * \code{.cpp}
 * Sprites sprites;  // Use this to optimize for execution speed
 * SpritesB sprites; // Use this to (likely) optimize for code size
 * \endcode
 * \endparblock
 *
 * \note
 * \parblock
 * In the example patterns given in each Sprites function description,
 * a # character represents a bit set to 1 and
 * a - character represents a bit set to 0.
 * \endparblock
 *
 * \see SpritesB
 */
class Sprites
{
  public:
    /** \brief
     * Draw a sprite using a separate image and mask array.
     *
     * \param x,y The coordinates of the top left pixel location.
     * \param bitmap A pointer to the array containing the image frames.
     * \param mask A pointer to the array containing the mask frames.
     * \param frame The frame number of the image to draw.
     * \param mask_frame The frame number for the mask to use (can be different
     * from the image frame number).
     *
     * \details
     * An array containing the image frames, and another array containing
     * corresponding mask frames, are used to draw a sprite.
     *
     * Bits set to 1 in the mask indicate that the pixel will be set to the
     * value of the corresponding image bit. Bits set to 0 in the mask will be
     * left unchanged.
     *
     *     image  mask   before  after  (# = 1, - = 0)
     *
     *     -----  -###-  -----   -----
     *     --#--  #####  -----   --#--
     *     ##-##  ##-##  -----   ##-##
     *     --#--  #####  -----   --#--
     *     -----  -###-  -----   -----
     *
     *     image  mask   before  after
     *
     *     -----  -###-  #####   #---#
     *     --#--  #####  #####   --#--
     *     ##-##  #####  #####   ##-##
     *     --#--  #####  #####   --#--
     *     -----  -###-  #####   #---#
     */
    static void drawExternalMask(int16_t x, int16_t y, const uint8_t *bitmap,
                                 const uint8_t *mask, uint8_t frame, uint8_t mask_frame);

    /** \brief
     * Draw a sprite using an array containing both image and mask values.
     *
     * \param x,y The coordinates of the top left pixel location.
     * \param bitmap A pointer to the array containing the image/mask frames.
     * \param frame The frame number of the image to draw.
     *
     * \details
     * An array containing combined image and mask data is used to draw a
     * sprite. Bytes are given in pairs with the first byte representing the
     * image pixels and the second byte specifying the corresponding mask.
     * The width given in the array still specifies the image width, so each
     * row of image and mask bytes will be twice the width value.
     *
     * Bits set to 1 in the mask indicate that the pixel will be set to the
     * value of the corresponding image bit. Bits set to 0 in the mask will be
     * left unchanged.
     *
     *     image  mask   before  after  (# = 1, - = 0)
     *
     *     -----  -###-  -----   -----
     *     --#--  #####  -----   --#--
     *     ##-##  ##-##  -----   ##-##
     *     --#--  #####  -----   --#--
     *     -----  -###-  -----   -----
     *
     *     image  mask   before  after
     *
     *     -----  -###-  #####   #---#
     *     --#--  #####  #####   --#--
     *     ##-##  #####  #####   ##-##
     *     --#--  #####  #####   --#--
     *     -----  -###-  #####   #---#
     */
    static void drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    /** \brief
     * Draw a sprite by replacing the existing content completely.
     *
     * \param x,y The coordinates of the top left pixel location.
     * \param bitmap A pointer to the array containing the image frames.
     * \param frame The frame number of the image to draw.
     *
     * \details
     * A sprite is drawn by overwriting the pixels in the buffer with the data
     * from the specified frame in the array. No masking is done. A bit set
     * to 1 in the frame will set the pixel to 1 in the buffer, and a 0 in the
     * array will set a 0 in the buffer.
     *
     *     image  before  after  (# = 1, - = 0)
     *
     *     -----  -----   -----
     *     --#--  -----   --#--
     *     ##-##  -----   ##-##
     *     --#--  -----   --#--
     *     -----  -----   -----
     *
     *     image  before  after
     *
     *     -----  #####   -----
     *     --#--  #####   --#--
     *     ##-##  #####   ##-##
     *     --#--  #####   --#--
     *     -----  #####   -----
     */
    static void drawOverwrite(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    /** \brief
     * "Erase" a sprite.
     *
     * \param x,y The coordinates of the top left pixel location.
     * \param bitmap A pointer to the array containing the image frames.
     * \param frame The frame number of the image to erase.
     *
     * \details
     * The data from the specified frame in the array is used to erase a
     * sprite. To "erase" a sprite, bits set to 1 in the frame will set the
     * corresponding pixel in the buffer to 0. Frame bits set to 0 will remain
     * unchanged in the buffer.
     *
     *     image  before  after  (# = 1, - = 0)
     *
     *     -----  -----   -----
     *     --#--  -----   -----
     *     ##-##  -----   -----
     *     --#--  -----   -----
     *     -----  -----   -----
     *
     *     image  before  after
     *
     *     -----  #####   #####
     *     --#--  #####   ##-##
     *     ##-##  #####   --#--
     *     --#--  #####   ##-##
     *     -----  #####   #####
     */
    static void drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    /** \brief
     * Draw a sprite using only the bits set to 1.
     *
     * \param x,y The coordinates of the top left pixel location.
     * \param bitmap A pointer to the array containing the image frames.
     * \param frame The frame number of the image to draw.
     *
     * \details
     * Bits set to 1 in the frame will be used to draw the sprite by setting
     * the corresponding pixel in the buffer to 1. Bits set to 0 in the frame
     * will remain unchanged in the buffer.
     *
     *     image  before  after  (# = 1, - = 0)
     *
     *     -----  -----   -----
     *     --#--  -----   --#--
     *     ##-##  -----   ##-##
     *     --#--  -----   --#--
     *     -----  -----   -----
     *
     *     image  before  after
     *
     *     -----  #####   #####  (no change because all pixels were
     *     --#--  #####   #####  already white)
     *     ##-##  #####   #####
     *     --#--  #####   #####
     *     -----  #####   #####
     */
    static void drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    // Master function. Needs to be abstracted into separate function for
    // every render type.
    // (Not officially part of the API)
    static void draw(int16_t x, int16_t y,
                     const uint8_t *bitmap, uint8_t frame,
                     const uint8_t *mask, uint8_t sprite_frame,
                     uint8_t drawMode);

    // (Not officially part of the API)
    static void drawBitmap(int16_t x, int16_t y,
                           const uint8_t *bitmap, const uint8_t *mask,
                           uint8_t w, uint8_t h, uint8_t draw_mode);
};

#endif

/**
 * @file Arduboy2Audio.h
 * \brief
 * The Arduboy2Audio class for speaker and sound control.
 */

#ifndef ARDUBOY2_AUDIO_H
#define ARDUBOY2_AUDIO_H

/** \brief
 * Provide speaker and sound control.
 *
 * \details
 * This class provides functions to initialize the speaker and control the
 * enabling and disabling (muting) of sound. It doesn't provide any functions
 * to actually produce sound.
 *
 * The state of sound muting is stored in system EEPROM and so is retained
 * over power cycles.
 *
 * An Arduboy2Audio class object named `audio` will be created by the
 * Arduboy2Base class, so there is no need for a sketch itself to create an
 * Arduboy2Audio object. Arduboy2Audio functions can be called using the
 * Arduboy2 or Arduboy2Base `audio` object.
 *
 * Example:
 *
 * \code{.cpp}
 * #include <Arduboy2.h>
 *
 * Arduboy2 arduboy;
 *
 * // Arduboy2Audio functions can be called as follows:
 *   arduboy.audio.on();
 *   arduboy.audio.off();
 * \endcode
 *
 * \note
 * \parblock
 * In order for this class to be fully functional, the external library or
 * functions used by a sketch to actually to produce sounds should be compliant
 * with this class. This means they should only produce sound if it is enabled,
 * or mute the sound if it's disabled. The `enabled()` function can be used
 * to determine if sound is enabled or muted. Generally a compliant library
 * would accept the `enabled()` function as an initialization parameter and
 * then call it as necessary to determine the current state.
 *
 * For example, the ArduboyTones and ArduboyPlaytune libraries require an
 * `enabled()` type function to be passed as a parameter in the constructor,
 * like so:
 *
 * \code{.cpp}
 * #include <Arduboy2.h>
 * #include <ArduboyTones.h>
 *
 * Arduboy2 arduboy;
 * ArduboyTones sound(arduboy.audio.enabled);
 * \endcode
 * \endparblock
 *
 * \note
 * \parblock
 * A friend class named _Arduboy2Ex_ is declared by this class. The intention
 * is to allow a sketch to create an _Arduboy2Ex_ class which would have access
 * to the private and protected members of the Arduboy2Audio class. It is hoped
 * that this may eliminate the need to create an entire local copy of the
 * library, in order to extend the functionality, in most circumstances.
 * \endparblock
 */
class Arduboy2Audio
{
 friend class Arduboy2Ex;

 public:
  /** \brief
   * Initialize the speaker based on the current mute setting.
   *
   * \details
   * The speaker is initialized based on the current mute setting saved in
   * system EEPROM. This function is called by `Arduboy2Base::begin()` so it
   * isn't normally required to call it within a sketch. However, if
   * `Arduboy2Core::boot()` is used instead of `Arduboy2Base::begin()` and the
   * sketch includes sound, then this function should be called after `boot()`.
   */
  void static begin();

  /** \brief
   * Turn sound on.
   *
   * \details
   * The system is configured to generate sound. This function sets the sound
   * mode only until the unit is powered off. To save the current mode use
   * `saveOnOff()`.
   *
   * \see off() toggle() saveOnOff()
   */
  void static on();

  /** \brief
   * Turn sound off (mute).
   *
   * \details
   * The system is configured to not produce sound (mute). This function sets
   * the sound mode only until the unit is powered off. To save the current
   * mode use `saveOnOff()`.
   *
   * \see on() toggle() saveOnOff()
   */
  void static off();

  /** \brief
   * Toggle the sound on/off state.
   *
   * \details
   * If the system is configured for sound on, it will be changed to sound off
   * (mute). If sound is off, it will be changed to on. This function sets
   * the sound mode only until the unit is powered off. To save the current
   * mode use `saveOnOff()`.
   *
   * \see on() off() saveOnOff()
   */
  void static toggle();

  /** \brief
   * Save the current sound state in EEPROM.
   *
   * \details
   * The current sound state, set by `on()` or `off()`, is saved to the
   * reserved system area in EEPROM. This allows the state to carry over between
   * power cycles and after uploading a different sketch.
   *
   * \note
   * EEPROM is limited in the number of times it can be written to. Sketches
   * should not continuously change and then save the state rapidly.
   *
   * \see on() off() toggle()
   */
  void static saveOnOff();

  /** \brief
   * Get the current sound state.
   *
   * \return `true` if sound is currently enabled (not muted).
   *
   * \details
   * This function should be used by code that actually generates sound.
   * If `true` is returned, sound can be produced. If `false` is returned,
   * sound should be muted.
   *
   * \see on() off() toggle()
   */
  bool static enabled();

 protected:
  bool static audio_enabled;
};

#endif

struct Rect
{
  int16_t x;
  int16_t y;
  uint8_t width;
  uint8_t height;
};

class Arduboy2Base
{
public:
    void begin();
    void setFrameRate(uint8_t rate);
    void initRandomSeed();
    bool everyXFrames(uint8_t frames);
    bool justPressed(uint8_t button);
    bool collide(Rect rect1, Rect rect2);
    bool nextFrame();
    void pollButtons();
    void clear();
    void display();

    Arduboy2Audio audio;
};

/**
 * @file ArduboyTones.h
 * \brief An Arduino library for playing tones and tone sequences, 
 * intended for the Arduboy game system.
 */

/*****************************************************************************
  ArduboyTones

An Arduino library to play tones and tone sequences.

Specifically written for use by the Arduboy miniature game system
https://www.arduboy.com/
but could work with other Arduino AVR boards that have 16 bit timer 3
available, by changing the port and bit definintions for the pin(s)
if necessary.

Copyright (c) 2017 Scott Allen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*****************************************************************************/

#ifndef ARDUBOY_TONES_H
#define ARDUBOY_TONES_H

// ************************************************************
// ***** Values to use as function parameters in sketches *****
// ************************************************************

/** \brief
 * Frequency value for sequence termination. (No duration follows)
 */
#define TONES_END 0x8000

/** \brief
 * Frequency value for sequence repeat. (No duration follows)
 */
#define TONES_REPEAT 0x8001


/** \brief
 * Add this to the frequency to play a tone at high volume
 */
#define TONE_HIGH_VOLUME 0x8000


/** \brief
 * `volumeMode()` parameter. Use the volume encoded in each tone's frequency
 */
#define VOLUME_IN_TONE 0

/** \brief
 * `volumeMode()` parameter. Play all tones at normal volume, ignoring
 * what's encoded in the frequencies
 */
#define VOLUME_ALWAYS_NORMAL 1

/** \brief
 * `volumeMode()` parameter. Play all tones at high volume, ignoring
 * what's encoded in the frequencies
 */
#define VOLUME_ALWAYS_HIGH 2

// ************************************************************


#ifndef AB_DEVKIT
  // ***** SPEAKER ON TWO PINS *****
  // Indicates that each of the speaker leads is attached to a pin, the way
  // the Arduboy is wired. Allows tones of a higher volume to be produced.
  // If commented out only one speaker pin will be used. The other speaker
  // lead should be attached to ground.
  #define TONES_2_SPEAKER_PINS
  // *******************************

  // ***** VOLUME HIGH/NORMAL SUPPORT *****
  // With the speaker placed across two pins, higher volume is produced by
  // toggling the second pin to the opposite state of the first pin.
  // Normal volume is produced by leaving the second pin low.
  // Comment this out for only normal volume support, which will slightly
  // reduce the code size.
  #define TONES_VOLUME_CONTROL
  // **************************************

  #ifdef TONES_VOLUME_CONTROL
    // Must be defined for volume control, so force it if necessary.
    #define TONES_2_SPEAKER_PINS
  #endif
#endif

// ***** CONTROL THE TIMER CLOCK PRESCALER ****
// Uncommenting this will switch the timer clock to use no prescaler,
// instead of a divide by 8 prescaler, if the frequency is high enough to
// allow it. This will result in higher frequencies being more accurate at
// the expense of requiring more code. If left commented out, a divide by 8
// prescaler will be used for all frequencies.
//#define TONES_ADJUST_PRESCALER
// ********************************************

// This must match the maximum number of tones that can be specified in
// the tone() function.
#define MAX_TONES 3

#ifndef AB_DEVKIT
  // Arduboy speaker pin 1 = Arduino pin 5 = ATmega32u4 PC6
  #define TONE_PIN_PORT PORTC
  #define TONE_PIN_DDR DDRC
  #define TONE_PIN PORTC6
  #define TONE_PIN_MASK _BV(TONE_PIN)
  // Arduboy speaker pin 2 = Arduino pin 13 = ATmega32u4 PC7
  #define TONE_PIN2_PORT PORTC
  #define TONE_PIN2_DDR DDRC
  #define TONE_PIN2 PORTC7
  #define TONE_PIN2_MASK _BV(TONE_PIN2)
#else
  // DevKit speaker pin 1 = Arduino pin A2 = ATmega32u4 PF5
  #define TONE_PIN_PORT PORTF
  #define TONE_PIN_DDR DDRF
  #define TONE_PIN PORTF5
  #define TONE_PIN_MASK _BV(TONE_PIN)
#endif

// The minimum frequency that can be produced without a clock prescaler.
#define MIN_NO_PRESCALE_FREQ ((uint16_t)(((F_CPU / 2L) + (1L << 16) - 1L) / (1L << 16)))

// Dummy frequency used to for silent tones (rests).
#define SILENT_FREQ 250


/** \brief
 * The ArduboyTones class for generating tones by specifying
 * frequency/duration pairs.
 */
class ArduboyTones
{
 public:
  /** \brief
   * The ArduboyTones class constructor.
   *
   * \param outEn A function which returns a boolean value of `true` if sound
   * should be played or `false` if sound should be muted. This function will
   * be called from the timer interrupt service routine, at the start of each
   * tone, so it should be as fast as possible.
   */
  ArduboyTones(bool (*outEn)());

  /** \brief
   * Play a single tone.
   *
   * \param freq The frequency of the tone, in hertz.
   * \param dur The duration to play the tone for, in 1024ths of a
   * second (very close to milliseconds). A duration of 0, or if not provided,
   * means play forever, or until `noTone()` is called or a new tone or
   * sequence is started.
   */
  static void tone(uint16_t freq, uint16_t dur = 0);

  /** \brief
   * Play two tones in sequence.
   *
   * \param freq1,freq2 The frequency of the tone in hertz.
   * \param dur1,dur2 The duration to play the tone for, in 1024ths of a
   * second (very close to milliseconds).
   */
  static void tone(uint16_t freq1, uint16_t dur1,
                   uint16_t freq2, uint16_t dur2);

  /** \brief
   * Play three tones in sequence.
   *
   * \param freq1,freq2,freq3 The frequency of the tone, in hertz.
   * \param dur1,dur2,dur3 The duration to play the tone for, in 1024ths of a
   * second (very close to milliseconds).
   */
  static void tone(uint16_t freq1, uint16_t dur1,
                   uint16_t freq2, uint16_t dur2,
                   uint16_t freq3, uint16_t dur3);

  /** \brief
   * Play a tone sequence from frequency/duration pairs in a PROGMEM array.
   *
   * \param tones A pointer to an array of frequency/duration pairs.
   * The array must be placed in code space using `PROGMEM`.
   *
   * \details 
   * \parblock
   * See the `tone()` function for details on the frequency and duration values.
   * A frequency of 0 for any tone means silence (a musical rest).
   *
   * The last element of the array must be `TONES_END` or `TONES_REPEAT`.
   *
   * Example:
   *
   * \code
   * const uint16_t sound1[] PROGMEM = {
   *   220,1000, 0,250, 440,500, 880,2000,
   *   TONES_END
   * };
   * \endcode
   *
   * \endparblock
   */
  static void tones(const uint16_t *tones);

  /** \brief
   * Play a tone sequence from frequency/duration pairs in an array in RAM.
   *
   * \param tones A pointer to an array of frequency/duration pairs.
   * The array must be located in RAM.
   *
   * \see tones()
   *
   * \details 
   * \parblock
   * See the `tone()` function for details on the frequency and duration values.
   * A frequency of 0 for any tone means silence (a musical rest).
   *
   * The last element of the array must be `TONES_END` or `TONES_REPEAT`.
   *
   * Example:
   *
   * \code
   * uint16_t sound2[] = {
   *   220,1000, 0,250, 440,500, 880,2000,
   *   TONES_END
   * };
   * \endcode
   *
   * \endparblock
   *
   * \note Using `tones()`, with the data in PROGMEM, is normally a better
   * choice. The only reason to use tonesInRAM() would be if dynamically
   * altering the contents of the array is required.
   */
  static void tonesInRAM(uint16_t *tones);

  /** \brief
   * Stop playing the tone or sequence.
   *
   * \details
   * If a tone or sequence is playing, it will stop. If nothing
   * is playing, this function will do nothing.
   */
  static void noTone();

  /** \brief
   * Set the volume to always normal, always high, or tone controlled.
   *
   * \param mode
   * \parblock
   * One of the following values should be used:
   *
   * - `VOLUME_IN_TONE` The volume of each tone will be specified in the tone
   *    itself.
   * - `VOLUME_ALWAYS_NORMAL` All tones will play at the normal volume level.
   * - `VOLUME_ALWAYS_HIGH` All tones will play at the high volume level.
   *
   * \endparblock
   */
  static void volumeMode(uint8_t mode);

  /** \brief
   * Check if a tone or tone sequence is playing.
   *
   * \return boolean `true` if playing (even if sound is muted).
   */
  static bool playing();

private:
  // Get the next value in the sequence
  static uint16_t getNext();

public:
  // Called from ISR so must be public. Should not be called by a program.
  static void nextTone();
};

#endif

/**
 * @file ArduboyTonesPitches.h
 * \brief Frequency definitions for standard note pitches.
 */

// Definitions ending with "H" indicate high volume

#ifndef ARDUBOY_TONES_PITCHES_H
#define ARDUBOY_TONES_PITCHES_H

#define NOTE_REST 0
#define NOTE_C0  16
#define NOTE_CS0 17
#define NOTE_D0  18
#define NOTE_DS0 19
#define NOTE_E0  21
#define NOTE_F0  22
#define NOTE_FS0 23
#define NOTE_G0  25
#define NOTE_GS0 26
#define NOTE_A0  28
#define NOTE_AS0 29
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2218
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define NOTE_E8  5274
#define NOTE_F8  5588
#define NOTE_FS8 5920
#define NOTE_G8  6272
#define NOTE_GS8 6645
#define NOTE_A8  7040
#define NOTE_AS8 7459
#define NOTE_B8  7902
#define NOTE_C9  8372
#define NOTE_CS9 8870
#define NOTE_D9  9397
#define NOTE_DS9 9956
#define NOTE_E9  10548
#define NOTE_F9  11175
#define NOTE_FS9 11840
#define NOTE_G9  12544
#define NOTE_GS9 13290
#define NOTE_A9  14080
#define NOTE_AS9 14917
#define NOTE_B9  15804

#define NOTE_C0H  (NOTE_C0 + TONE_HIGH_VOLUME)
#define NOTE_CS0H (NOTE_CS0 + TONE_HIGH_VOLUME)
#define NOTE_D0H  (NOTE_D08 + TONE_HIGH_VOLUME)
#define NOTE_DS0H (NOTE_DS0 + TONE_HIGH_VOLUME)
#define NOTE_E0H  (NOTE_E0 + TONE_HIGH_VOLUME)
#define NOTE_F0H  (NOTE_F0 + TONE_HIGH_VOLUME)
#define NOTE_FS0H (NOTE_FS0 + TONE_HIGH_VOLUME)
#define NOTE_G0H  (NOTE_G0 + TONE_HIGH_VOLUME)
#define NOTE_GS0H (NOTE_GS0 + TONE_HIGH_VOLUME)
#define NOTE_A0H  (NOTE_A0 + TONE_HIGH_VOLUME)
#define NOTE_AS0H (NOTE_AS0 + TONE_HIGH_VOLUME)
#define NOTE_B0H  (NOTE_B0 + TONE_HIGH_VOLUME)
#define NOTE_C1H  (NOTE_C1 + TONE_HIGH_VOLUME)
#define NOTE_CS1H (NOTE_CS1 + TONE_HIGH_VOLUME)
#define NOTE_D1H  (NOTE_D1 + TONE_HIGH_VOLUME)
#define NOTE_DS1H (NOTE_DS1 + TONE_HIGH_VOLUME)
#define NOTE_E1H  (NOTE_E1 + TONE_HIGH_VOLUME)
#define NOTE_F1H  (NOTE_F1 + TONE_HIGH_VOLUME)
#define NOTE_FS1H (NOTE_FS1 + TONE_HIGH_VOLUME)
#define NOTE_G1H  (NOTE_G1 + TONE_HIGH_VOLUME)
#define NOTE_GS1H (NOTE_GS1 + TONE_HIGH_VOLUME)
#define NOTE_A1H  (NOTE_A1 + TONE_HIGH_VOLUME)
#define NOTE_AS1H (NOTE_AS1 + TONE_HIGH_VOLUME)
#define NOTE_B1H  (NOTE_B1 + TONE_HIGH_VOLUME)
#define NOTE_C2H  (NOTE_C2 + TONE_HIGH_VOLUME)
#define NOTE_CS2H (NOTE_CS2 + TONE_HIGH_VOLUME)
#define NOTE_D2H  (NOTE_D2 + TONE_HIGH_VOLUME)
#define NOTE_DS2H (NOTE_DS2 + TONE_HIGH_VOLUME)
#define NOTE_E2H  (NOTE_E2 + TONE_HIGH_VOLUME)
#define NOTE_F2H  (NOTE_F2 + TONE_HIGH_VOLUME)
#define NOTE_FS2H (NOTE_FS2 + TONE_HIGH_VOLUME)
#define NOTE_G2H  (NOTE_G2 + TONE_HIGH_VOLUME)
#define NOTE_GS2H (NOTE_GS2 + TONE_HIGH_VOLUME)
#define NOTE_A2H  (NOTE_A2 + TONE_HIGH_VOLUME)
#define NOTE_AS2H (NOTE_AS2 + TONE_HIGH_VOLUME)
#define NOTE_B2H  (NOTE_B2 + TONE_HIGH_VOLUME)
#define NOTE_C3H  (NOTE_C3 + TONE_HIGH_VOLUME)
#define NOTE_CS3H (NOTE_CS3 + TONE_HIGH_VOLUME)
#define NOTE_D3H  (NOTE_D3 + TONE_HIGH_VOLUME)
#define NOTE_DS3H (NOTE_DS3 + TONE_HIGH_VOLUME)
#define NOTE_E3H  (NOTE_E3 + TONE_HIGH_VOLUME)
#define NOTE_F3H  (NOTE_F3 + TONE_HIGH_VOLUME)
#define NOTE_FS3H (NOTE_F3 + TONE_HIGH_VOLUME)
#define NOTE_G3H  (NOTE_G3 + TONE_HIGH_VOLUME)
#define NOTE_GS3H (NOTE_GS3 + TONE_HIGH_VOLUME)
#define NOTE_A3H  (NOTE_A3 + TONE_HIGH_VOLUME)
#define NOTE_AS3H (NOTE_AS3 + TONE_HIGH_VOLUME)
#define NOTE_B3H  (NOTE_B3 + TONE_HIGH_VOLUME)
#define NOTE_C4H  (NOTE_C4 + TONE_HIGH_VOLUME)
#define NOTE_CS4H (NOTE_CS4 + TONE_HIGH_VOLUME)
#define NOTE_D4H  (NOTE_D4 + TONE_HIGH_VOLUME)
#define NOTE_DS4H (NOTE_DS4 + TONE_HIGH_VOLUME)
#define NOTE_E4H  (NOTE_E4 + TONE_HIGH_VOLUME)
#define NOTE_F4H  (NOTE_F4 + TONE_HIGH_VOLUME)
#define NOTE_FS4H (NOTE_FS4 + TONE_HIGH_VOLUME)
#define NOTE_G4H  (NOTE_G4 + TONE_HIGH_VOLUME)
#define NOTE_GS4H (NOTE_GS4 + TONE_HIGH_VOLUME)
#define NOTE_A4H  (NOTE_A4 + TONE_HIGH_VOLUME)
#define NOTE_AS4H (NOTE_AS4 + TONE_HIGH_VOLUME)
#define NOTE_B4H  (NOTE_B4 + TONE_HIGH_VOLUME)
#define NOTE_C5H  (NOTE_C5 + TONE_HIGH_VOLUME)
#define NOTE_CS5H (NOTE_CS5 + TONE_HIGH_VOLUME)
#define NOTE_D5H  (NOTE_D5 + TONE_HIGH_VOLUME)
#define NOTE_DS5H (NOTE_DS5 + TONE_HIGH_VOLUME)
#define NOTE_E5H  (NOTE_E5 + TONE_HIGH_VOLUME)
#define NOTE_F5H  (NOTE_F5 + TONE_HIGH_VOLUME)
#define NOTE_FS5H (NOTE_FS5 + TONE_HIGH_VOLUME)
#define NOTE_G5H  (NOTE_G5 + TONE_HIGH_VOLUME)
#define NOTE_GS5H (NOTE_GS5 + TONE_HIGH_VOLUME)
#define NOTE_A5H  (NOTE_A5 + TONE_HIGH_VOLUME)
#define NOTE_AS5H (NOTE_AS5 + TONE_HIGH_VOLUME)
#define NOTE_B5H  (NOTE_B5 + TONE_HIGH_VOLUME)
#define NOTE_C6H  (NOTE_C6 + TONE_HIGH_VOLUME)
#define NOTE_CS6H (NOTE_CS6 + TONE_HIGH_VOLUME)
#define NOTE_D6H  (NOTE_D6 + TONE_HIGH_VOLUME)
#define NOTE_DS6H (NOTE_DS6 + TONE_HIGH_VOLUME)
#define NOTE_E6H  (NOTE_E6 + TONE_HIGH_VOLUME)
#define NOTE_F6H  (NOTE_F6 + TONE_HIGH_VOLUME)
#define NOTE_FS6H (NOTE_FS6 + TONE_HIGH_VOLUME)
#define NOTE_G6H  (NOTE_G6 + TONE_HIGH_VOLUME)
#define NOTE_GS6H (NOTE_GS6 + TONE_HIGH_VOLUME)
#define NOTE_A6H  (NOTE_A6 + TONE_HIGH_VOLUME)
#define NOTE_AS6H (NOTE_AS6 + TONE_HIGH_VOLUME)
#define NOTE_B6H  (NOTE_B6 + TONE_HIGH_VOLUME)
#define NOTE_C7H  (NOTE_C7 + TONE_HIGH_VOLUME)
#define NOTE_CS7H (NOTE_CS7 + TONE_HIGH_VOLUME)
#define NOTE_D7H  (NOTE_D7 + TONE_HIGH_VOLUME)
#define NOTE_DS7H (NOTE_DS7 + TONE_HIGH_VOLUME)
#define NOTE_E7H  (NOTE_E7 + TONE_HIGH_VOLUME)
#define NOTE_F7H  (NOTE_F7 + TONE_HIGH_VOLUME)
#define NOTE_FS7H (NOTE_FS7 + TONE_HIGH_VOLUME)
#define NOTE_G7H  (NOTE_G7 + TONE_HIGH_VOLUME)
#define NOTE_GS7H (NOTE_GS7 + TONE_HIGH_VOLUME)
#define NOTE_A7H  (NOTE_A7 + TONE_HIGH_VOLUME)
#define NOTE_AS7H (NOTE_AS7 + TONE_HIGH_VOLUME)
#define NOTE_B7H  (NOTE_B7 + TONE_HIGH_VOLUME)
#define NOTE_C8H  (NOTE_C8 + TONE_HIGH_VOLUME)
#define NOTE_CS8H (NOTE_CS8 + TONE_HIGH_VOLUME)
#define NOTE_D8H  (NOTE_D8 + TONE_HIGH_VOLUME)
#define NOTE_DS8H (NOTE_DS8 + TONE_HIGH_VOLUME)
#define NOTE_E8H  (NOTE_E8 + TONE_HIGH_VOLUME)
#define NOTE_F8H  (NOTE_F8 + TONE_HIGH_VOLUME)
#define NOTE_FS8H (NOTE_FS8 + TONE_HIGH_VOLUME)
#define NOTE_G8H  (NOTE_G8 + TONE_HIGH_VOLUME)
#define NOTE_GS8H (NOTE_GS8 + TONE_HIGH_VOLUME)
#define NOTE_A8H  (NOTE_A8 + TONE_HIGH_VOLUME)
#define NOTE_AS8H (NOTE_AS8 + TONE_HIGH_VOLUME)
#define NOTE_B8H  (NOTE_B8 + TONE_HIGH_VOLUME)
#define NOTE_C9H  (NOTE_C9 + TONE_HIGH_VOLUME)
#define NOTE_CS9H (NOTE_CS9 + TONE_HIGH_VOLUME)
#define NOTE_D9H  (NOTE_D9 + TONE_HIGH_VOLUME)
#define NOTE_DS9H (NOTE_DS9 + TONE_HIGH_VOLUME)
#define NOTE_E9H  (NOTE_E9 + TONE_HIGH_VOLUME)
#define NOTE_F9H  (NOTE_F9 + TONE_HIGH_VOLUME)
#define NOTE_FS9H (NOTE_FS9 + TONE_HIGH_VOLUME)
#define NOTE_G9H  (NOTE_G9 + TONE_HIGH_VOLUME)
#define NOTE_GS9H (NOTE_GS9 + TONE_HIGH_VOLUME)
#define NOTE_A9H  (NOTE_A9 + TONE_HIGH_VOLUME)
#define NOTE_AS9H (NOTE_AS9 + TONE_HIGH_VOLUME)
#define NOTE_B9H  (NOTE_B9 + TONE_HIGH_VOLUME)

#endif
