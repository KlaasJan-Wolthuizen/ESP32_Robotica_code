//
// Created by User on 31-5-2023.
//
#include <Arduino.h>
#include <sstream>
#include <string>

#include "SBC_Com.hpp"
#include "commands.h"
#include "Pin_setup.hpp"
#include "PowerController.hpp"
#include "Motor_functions.hpp"
#include "Lights.hpp"


/* Variable initialization */
// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index_arg = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[16];  //Here the parameter values are stored
char argv2[16];  //Here the parameter values are stored
char argv3[16];  //Here the parameter values are stored

// The arguments converted to integers
long arg1;
long arg2;
long arg3;

extern uint8_t * barHeight;

uint8_t * band_pointer;

extern display_mode mode_SBC;

void init_serial() {
    Serial.begin(BAUDRATE);
//    Serial.println("\n\rStarted!\nWanted value = ");
    //Serial.println(wanted_value, DEC);
}


void comm_SBC() {
    while (Serial.available() > 0) {
        // Read the next character
        chr = Serial.read();

        // Terminate a command with a CR
        if (chr == 13) {
            if (arg == 1) argv1[index_arg] = NULL;
            else if (arg == 2) argv2[index_arg] = NULL;
            else if (arg == 3) argv3[index_arg] = NULL;
            run_command();
            reset_command();
        }
            // Use spaces to delimit parts of the command
        else if (chr == ' ') {
            // Step through the arguments
            if (arg == 0) arg = 1;
            else if (arg == 1) {
                argv1[index_arg] = NULL;
                arg = 2;
                index_arg = 0;
            }else if (arg == 2) {
                argv2[index_arg] = NULL;
                arg = 3;
                index_arg = 0;
            }
            continue;
        } else {
            if (arg == 0) {
                // The first arg is the single-letter command
                cmd = chr;
            } else if (arg == 1) {
                // Subsequent arguments can be more than one character
                argv1[index_arg] = chr;
                index_arg++;
            } else if (arg == 2) {
                argv2[index_arg] = chr;
                index_arg++;
            } else if (arg == 3) {
                argv3[index_arg] = chr;
                index_arg++;
            }
        }
    }
}

int run_command() {
    //convert give string numbers to integers
    arg1 = atoi(argv1);
    arg2 = atoi(argv2);
    arg3 = atoi(argv3);

    switch (cmd) {
        case CONTROL_LED: // Used for testing purposes
            digitalWrite(LED_BUILTIN, arg1);
            break;

        case MOTOR_SPEEDS:  //Command to control motors
            simple_left_DC_motor_control((int16_t) -arg1);  //set motor speed of motors on the left side
            simple_right_DC_motor_control((int16_t) -arg2); //set motor speed of motors on the right side
            simple_track_DC_motor_control((int16_t) arg3); //set motor speed of track
            break;

        case GET_SENSORS:
            char sensor_data[50];
            sprintf(sensor_data, "%c %.2f %.2f\n", GET_SENSORS, get_bat_voltage(), get_bat_current()); //format return message to SBC
            Serial.print(sensor_data);  //send return message
            break;

        case MATRIX:
            //change matrix to mode given by command argument
            mode_SBC = (display_mode)arg1; //sets mode of function display_matrix(mode_SBC), this function runs in main loop.
            break;

        case STATE:
            ;// not implemented
            break;

        case GET_SOUND_SPECT:
            //send bands from VU to SBC
            band_pointer = get_band();
            char sound_data[50];
            sprintf(sound_data, "%c %i %i %i\n", GET_SOUND_SPECT, *band_pointer, *(band_pointer+1), *(band_pointer+2)); //format return message to SBC
            Serial.print(sound_data); //send return message
            break;
    }
    return 0;
}


void reset_command() {
    //reset SBC_com variables
    cmd = NULL;
    memset(argv1, 0, sizeof(argv1));
    memset(argv2, 0, sizeof(argv2));
    arg1 = 0;
    arg2 = 0;
    arg3 = 0;
    arg = 0;
    index_arg = 0;
}


void send_debug_data(char* topic, char* message) {
    #ifdef DEBUG_MODE
        Serial.print("[DEBUG INFO]\t[");
        Serial.print(topic);
        Serial.print("]\t");
        Serial.println(message);
    #endif
}