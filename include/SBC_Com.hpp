//
// Created by Klaas Jan on 31-5-2023.
//

#ifndef ESP32_ROBOTICA_CODE_SBC_COM_HPP
#define ESP32_ROBOTICA_CODE_SBC_COM_HPP

#include <Arduino.h>
#include "string.h"

#define DEBUG_MODE

#define BAUDRATE 115200

void init_serial();

void comm_SBC();

int run_command();
void reset_command();

void send_debug_data(char* topic, char* message);


#endif //ESP32_ROBOTICA_CODE_SBC_COM_HPP
