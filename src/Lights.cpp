//
// Created by User on 4-6-2023.
//

#include "Lights.hpp"
#include "../.pio/libdeps/esp32doit-devkit-v1/Adafruit NeoMatrix/Adafruit_NeoMatrix.h"
#include "../.pio/libdeps/esp32doit-devkit-v1/Adafruit NeoPixel/Adafruit_NeoPixel.h"
#include "../.pio/libdeps/esp32doit-devkit-v1/Adafruit GFX Library/Adafruit_GFX.h"


// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:

//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

#define PUPIL_COLOR 0x02df
#define EYES_COLOR 0x07f7

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
        8, 8, 2, 1, ESP32_PIN_MATRIX_DATA,  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
                                            NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE +
                                            NEO_TILE_TOP + NEO_TILE_LEFT +
                                            NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB + NEO_KHZ800);

unsigned long light_millis;
display_mode previous_state;


//FFT variables and object declaration
unsigned int sampling_period_us;
double vReal[SAMPLES];
double vImag[SAMPLES];
int bandValues[] = {0,0,0};
//int oldBarHeights[] = {0,0,0};
uint8_t barHeight[NUM_BANDS];
unsigned long newTime;
uint8_t * bandHeight_ptr;

arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);

//TaskHandle_t calc_spect;


void init_lights() {
    light_millis = millis();
    sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ)); //sets the time per sample in microseconds
    matrix.begin(); //initializes the led Matrix
    //matrix.setTextWrap(false);
    matrix.setBrightness(BRIGHTNESS); // Sets brightness for the matrix
    matrix.fillScreen(0x0000); //sets all pixels to no color
    matrix.show(); //updates the matrix.
}

 //function to make an top eyebrow above on selected eye
void TopEyeBrow (uint8_t eyeNr, eyebrow_shape shape, int8_t height) {
    switch (shape) {
        case Straight:
            for(int i=0; i < height; i++) {
                matrix.drawLine(eyeNr*8, i, eyeNr*8+7, i, 0x0000);
            }
            break;
        case Left_first:
            ; // not implemented
            break;
        case Right_first:
            break;
        case Circle:
            break;
    }
}

void light() { //function to create a lot of light, used when vision camera needs extra light
    matrix.setBrightness(255); // Sets brightness to maximum for this function.
    matrix.fillScreen(0xffff); //make full matrix white
    matrix.show();
}

void display_matrix(display_mode state) {
    matrix.setBrightness(BRIGHTNESS); // Sets brightness for the matrix
    switch (state) {
        case Display_off:
            matrix.fillScreen(0x0000);
            matrix.show();
            break;
        case VU:
            //add FFT
            bandHeight_ptr = get_band();
            display_VU(*bandHeight_ptr, *(bandHeight_ptr+1), *(bandHeight_ptr+2));
            break;
        case Light:
            light();
            break;
        case Normal:
            normal_eyes_anim();
            break;
        case Happy:
            break; //not implemented
        case Sad:
            break;  //not implemented
        case Mad:
            if ((previous_state != state) && (previous_state == Mad)) {
                // start animation of retracting eye brow
                mad_stop();
            } else if ((previous_state != state) && (previous_state != Mad)) {
                // start animation of eye brow going down to a mad state
                mad_start();
            } else {
                ; //do nothing
            }
            break;

        case Concentrated:
            if ((previous_state != state) && (previous_state == Concentrated)) {
                // start animation of retracting eye brow
                concentrate_stop();
            } else if ((previous_state != state) && (previous_state != Concentrated)) {
                // start animation of eye brow going down to a concentrated state
                concentrate_start();
            }
            else {
                ; //do nothing
            }
            break;
    }
    previous_state = state;
}


void normal_eyes_anim() {
    //animation of eyes, looking around every few seconds
    if (millis()-light_millis > 3000 && millis()-light_millis < 4000) {
        //eyes looking straight
        matrix.fillScreen(0x0000);
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(1, 1,PUPIL_COLOR);
        create_pupil(9, 1, PUPIL_COLOR);
        matrix.show();
    } else if (millis()-light_millis > 4000 && millis()-light_millis < 6000) {
        //eyes looking left
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(0, 1,PUPIL_COLOR);
        create_pupil(8, 1, PUPIL_COLOR);
        matrix.show();
    } else if (millis()-light_millis > 6000 && millis()-light_millis < 7500) {
        //eyes looking straight
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(1, 1,PUPIL_COLOR);
        create_pupil(9, 1, PUPIL_COLOR);
        matrix.show();
    } else if (millis()-light_millis > 7500 && millis()-light_millis < 8500) {
        // eyes looking right
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(2, 1, PUPIL_COLOR);
        create_pupil(10, 1, PUPIL_COLOR);
        matrix.show();
    } else if (millis()-light_millis > 8500) {
        //eyes looking straight
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(1, 1,PUPIL_COLOR);
        create_pupil(9, 1, PUPIL_COLOR);
        matrix.show();
        light_millis = millis();
    }
}

void normal_eyes_anim_delay() { //same function as normal_eyes_anim(), but makes use of vTaskDelay, useful when running this function as a seperate task
    create_eyes(1, 0, EYES_COLOR);
    create_eyes(9, 0, EYES_COLOR);
    create_pupil(1, 1,PUPIL_COLOR);
    create_pupil(9, 1, PUPIL_COLOR);
    matrix.show();
    vTaskDelay(3000/portTICK_PERIOD_MS);

    create_eyes(1, 0, EYES_COLOR);
    create_eyes(9, 0, EYES_COLOR);
    create_pupil(0, 1,PUPIL_COLOR);
    create_pupil(8, 1, PUPIL_COLOR);
    matrix.show();
    vTaskDelay(1000/portTICK_PERIOD_MS);

    create_eyes(1, 0, EYES_COLOR);
    create_eyes(9, 0, EYES_COLOR);
    create_pupil(1, 1,PUPIL_COLOR);
    create_pupil(9, 1, PUPIL_COLOR);
    matrix.show();
    vTaskDelay(2000/portTICK_PERIOD_MS);

    create_eyes(1, 0, EYES_COLOR);
    create_eyes(9, 0, EYES_COLOR);
    create_pupil(2, 1,PUPIL_COLOR);
    create_pupil(10, 1, PUPIL_COLOR);
    matrix.show();
    vTaskDelay(1500/portTICK_PERIOD_MS);

    create_eyes(1, 0, EYES_COLOR);
    create_eyes(9, 0, EYES_COLOR);
    create_pupil(1, 1,PUPIL_COLOR);
    create_pupil(9, 1, PUPIL_COLOR);
    matrix.show();
    vTaskDelay(1000/portTICK_PERIOD_MS);
}


void mad_start() {
    for (int8_t i = 0; i < 3; i++) {
        TopEyeBrow(0, Straight, i);
        TopEyeBrow(1, Straight, i);
        matrix.show();
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}


void mad_stop() {
    for (int8_t i = 3; i > 0; i--) {
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(1, 1,PUPIL_COLOR);
        create_pupil(9, 1, PUPIL_COLOR);
        TopEyeBrow(0, Straight, i);
        TopEyeBrow(1, Straight, i);
        matrix.show();
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}


void concentrate_start() {
    for (int8_t i = 0; i < 3; i++) {
        TopEyeBrow(0, Straight, i);
        TopEyeBrow(1, Straight, i);
        matrix.show();
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}


void concentrate_stop() {
    for (int8_t i = 3; i > 0; i--) {
        create_eyes(1, 0, EYES_COLOR);
        create_eyes(9, 0, EYES_COLOR);
        create_pupil(1, 1,PUPIL_COLOR);
        create_pupil(9, 1, PUPIL_COLOR);
        TopEyeBrow(0, Straight, i);
        TopEyeBrow(1, Straight, i);
        matrix.show();
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}


//void animation() {
// under development
//    for (int8_t i = 0; i < 3; i++) {
//        TopEyeBrow(0, Straight, i);
//        TopEyeBrow(1, Straight, i);
//        matrix.show();
//        vTaskDelay(200/portTICK_PERIOD_MS);
//    }
//    create_eyes(1, 0, EYES_COLOR);
//    create_eyes(9, 0, EYES_COLOR);
//    create_pupil(1, 1,PUPIL_COLOR);
//    create_pupil(9, 1, PUPIL_COLOR);
//    matix.show();
//    vTaskDelay(4000/portTICK_PERIOD_MS);
//
//    for (int8_t i = 3; i > 0; i--) {
//        create_eyes(1, 0, EYES_COLOR);
//        create_eyes(9, 0, EYES_COLOR);
//        create_pupil(1, 1,PUPIL_COLOR);
//        create_pupil(9, 1, PUPIL_COLOR);
//        TopEyeBrow(0, Straight, i);
//        TopEyeBrow(1, Straight, i);
//        matrix.show();
//        vTaskDelay(200/portTICK_PERIOD_MS);
//    }
//}

void display_VU(uint8_t low_band, uint8_t middle_band, uint8_t high_band) {
    //function to display frequency band amplitudes.
    matrix.fillScreen(0x0000); //clears the matrix

    //draw the low bands om 2 colloms
    matrix.drawFastVLine(0, 8, 1-low_band, 0xf800);
    matrix.drawFastVLine(1, 8, 1-low_band, 0xf800);

    //draw the middle bands om 2 colloms
    matrix.drawFastVLine(2, 8, 1-middle_band, 0x07e0);
    matrix.drawFastVLine(3, 8, 1-middle_band, 0x07e0);

    //draw the high bands om 2 colloms
    matrix.drawFastVLine(4, 8, 1-high_band, 0x001f);
    matrix.drawFastVLine(5, 8, 1-high_band, 0x001f);

    //draw the same bands on the second matrix board but mirrored horizontally
    matrix.drawFastVLine(15, 8, 1-low_band, 0xf800);
    matrix.drawFastVLine(14, 8, 1-low_band, 0xf800);
    matrix.drawFastVLine(13, 8, 1-middle_band, 0x07e0);
    matrix.drawFastVLine(12, 8, 1-middle_band, 0x07e0);
    matrix.drawFastVLine(11, 8, 1-high_band, 0x001f);
    matrix.drawFastVLine(10, 8, 1-high_band, 0x001f);

    matrix.show(); //update matrix
}

//function to create eyes (without pupil, seperate function for that)
void create_eyes(uint8_t posX, uint8_t posY, short color) {
    matrix.fillCircle(posX+2, posY+2, 2, color);
    matrix.fillCircle(posX+3, posY+2, 2, color);
    matrix.fillCircle(posX+2, posY+4, 2, color);
    matrix.fillCircle(posX+3, posY+4, 2, color);
}


//function to create pupils
void create_pupil(uint8_t posX, uint8_t posY, short color) {
    matrix.fillCircle(posX+2, posY+1, 1, color);
    matrix.fillCircle(posX+3, posY+1, 1, color);
    matrix.fillCircle(posX+2, posY+2, 1, color);
    matrix.fillCircle(posX+3, posY+2, 1, color);
    matrix.fillCircle(posX+2, posY+3, 1, color);
    matrix.fillCircle(posX+3, posY+3, 1, color);
}


//void text_anmition (){
//    matrix.fillScreen(0);
//    matrix.setCursor(x, 0);
//    matrix.print(F("TerraMedic"));
//    if(--x < -36) {
//        x = matrix.width();
//        if(++pass >= 3) pass = 0;
//        matrix.setTextColor(colors[pass]);
//    }
//    matrix.show();
//    delay(100);
//}

//void run_UV() {
//   //create task and run on core 1
//}
//
//void stop_UV() {
//    //delete task and
//}


//function heavily inspired from Scott Marley, see https://github.com/s-marley/ESP32_FFT_VU/tree/master
uint8_t * get_band() {  //function to get calculate the amplitude for 3 frequeny bands, calculation is done on a analog microphone signal.,
    // Reset bandValues[]
    for (int i = 0; i < NUM_BANDS; i++) {
        bandValues[i] = 0;
    }

    // Sample the audio pin
    for (int i = 0; i < SAMPLES; i++) {
        newTime = micros();
        vReal[i] = analogRead(ESP32_PIN_MIC_IN); // A conversion takes about 9.7uS on an ESP32
        vImag[i] = 0;
        while ((micros() - newTime) < sampling_period_us) { /* wait for next sample moment */ }
    }

    // Compute FFT
    FFT.DCRemoval(); //Remove the DC component from audio signal
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); //sets the window to hamming type
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude(); //calculates the magnitudes af signal for every bin

    // Analyse FFT results
    for (int i = 2; i < (SAMPLES / 2); i++) {       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
//        if (vReal[i] > NOISE) {                    // Add a crude noise filter
            // 8 bands, 12kHz top band
            if (i <= 22) bandValues[0] += (int) vReal[i];
            if (i > 22 && i <= 230) bandValues[1] += (int) vReal[i];
            if (i > 230) bandValues[2] += (int) vReal[i];
//        }
    }

    // Process the FFT data into bar heights
    for (byte band = 0; band < NUM_BANDS; band++) {
      // calcutate the average amplitude and divide by number of bins to get usable results for the led matrix
        barHeight[0] = bandValues[0] / ((22-2)*AMPLITUDE);
        barHeight[1] = bandValues[1] / ((230-22)*AMPLITUDE);
        barHeight[2] = bandValues[2] / ((512-230)*AMPLITUDE);

//        barHeight[0] = bandValues[0] / ((22-2)*AMPLITUDE*5);
//        barHeight[1] = bandValues[1] / ((230-22)*AMPLITUDE);
//        barHeight[2] = bandValues[2] / ((512-230)*AMPLITUDE);
        if (barHeight[band] > TOP) barHeight[band] = TOP; //clips the barheight when value is too high.

        // Small amount of averaging between frames
//        barHeight[band] = ((oldBarHeights[band] * 1) + barHeight[band]) / 2;

        // Move peak up
//        if (barHeight[band] > peak[band]) {
//            peak[band] = min(TOP, barHeight[band]);
//        }
    }
    return barHeight;
//    for (byte band = 0; band < NUM_BANDS; band++) {
//        oldBarHeights[band] = barHeight[band];
//    };
}


void display_bat_low() {
    matrix.setBrightness(BRIGHTNESS);
    matrix.fillScreen(0x0000); //clear screen

    //indicate low battery voltage by 2 crosses.
    matrix.drawLine(0, 0 ,7, 7, 0xf800);
    matrix.drawLine(0, 7, 7, 0, 0xf800);
    matrix.drawLine(8, 0 ,15, 7, 0xf800);
    matrix.drawLine(8, 7, 15, 0, 0xf800);
    matrix.show();
}