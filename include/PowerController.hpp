//
// Created by User on 29-5-2023.
//

#ifndef ESP32_ROBOTICA_CODE_POWERCONTROLLER_HPP
#define ESP32_ROBOTICA_CODE_POWERCONTROLLER_HPP

#include <Arduino.h>
#include "Pin_setup.hpp"

void powerController();

float_t get_bat_voltage();
float_t get_bat_current();
void start_SBC();
void shutdown_SBC();

#endif //ESP32_ROBOTICA_CODE_POWERCONTROLLER_HPP
