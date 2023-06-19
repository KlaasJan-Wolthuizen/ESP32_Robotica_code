//
// Created by User on 4-6-2023.
//

#ifndef ESP32_ROBOTICA_CODE_LIGHTS_HPP
#define ESP32_ROBOTICA_CODE_LIGHTS_HPP

//#include "C:\Users\User\NHL Stenden\Group - Project P8 - Robotica - Elektrotechniek - Elektrotechniek\05. Programma\ESP32_Robotica_code\.pio\libdeps\esp32doit-devkit-v1\Adafruit NeoPixel\Adafruit_NeoPixel.h"
//#include "C:\Users\User\NHL Stenden\Group - Project P8 - Robotica - Elektrotechniek - Elektrotechniek\05. Programma\ESP32_Robotica_code\.pio\libdeps\esp32doit-devkit-v1\Adafruit NeoMatrix\Adafruit_NeoMatrix.h"
//#include <C:\Users\User\NHL Stenden\Group - Project P8 - Robotica - Elektrotechniek - Elektrotechniek\05. Programma\ESP32_Robotica_code\.pio\libdeps\esp32doit-devkit-v1\Adafruit GFX Library\Adafruit_GFX.h>

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>
#include <ArduinoFFT.h>

#include "Pin_setup.hpp"


#define BRIGHTNESS 255

//FFT Settings
#define SAMPLES         1024    // Must be a power of 2
#define SAMPLING_FREQ   40000   // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define NUM_BANDS       3            // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define NOISE           500           // Used as a crude noise filter, values below this are ignored
#define AMPLITUDE       7000          // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define TOP             8


typedef enum {
    Straight,
    Left_first,
    Right_first,
    Circle} eyebrow_shape;

typedef enum {Display_off, VU, Light, Normal, Happy, Sad, Mad, Concentrated} display_mode;

void init_lights();

void display_matrix(display_mode state);

void display_VU(uint8_t low_band, uint8_t middle_band, uint8_t high_band);

//void animation(); //under development
void normal_eyes_anim();
void light();

void TopEyeBrow(uint8_t eyeNr, eyebrow_shape shape, int8_t height);
void create_eyes(uint8_t posX, uint8_t posY, short color);
void create_pupil(uint8_t posX, uint8_t posY, short color);

void mad_start();
void mad_stop();

void concentrate_start();
void concentrate_stop();

void display_bat_low();

uint8_t * get_band();


#endif //ESP32_ROBOTICA_CODE_LIGHTS_HPP
