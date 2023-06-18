//
// Created by Klaas Jan on 29-5-2023.
//

#ifndef ESP32_ROBOTICA_CODE_PIN_SETUP_HPP
#define ESP32_ROBOTICA_CODE_PIN_SETUP_HPP

#include <Arduino.h>

void ESP32_pin_setup();

// Powersystem control and monitoring pins
#define ESP32_PIN_BAT_MEAS_CURRENT 36
#define ESP32_PIN_BAT_MEAS_VOLTAGE 39
#define ESP32_PIN_START_SHUT_SBC 15
#define ESP32_PIN_SHUT_REQ_SBC 23
#define ESP32_PIN_SHUT_REQ 34
#define ESP32_PIN_EN_MAIN_SW 32

// LED Strip pins
#define ESP32_PIN_MATRIX_DATA 19
#define ESP32_PIN_STRIP1_DATA 18
#define ESP32_PIN_STRIP2_DATA 5

// Audio pin
#define ESP32_PIN_MIC_IN  35

//SIPO(Serial In Parallel Out) Shift register pins (74HC595N)
#define ESP32_PIN_SIPO_SRCLK 12         // Serial clock output pin
#define ESP32_PIN_SIPO_DATA 27          // Serial data input
#define ESP32_PIN_SIPO_RCLK 14      // Latch pin, tranfers shift register data to storage register

// DC Motor ESP Pins
#define ESP32_PIN_DC_MOTOR_LEFT_PWM 25
#define ESP32_PIN_DC_MOTOR_RIGHT_PWM 26
#define ESP32_PIN_DC_MOTOR_TRACK_PWM 13
#define ESP32_PIN_DC_MOTOR_LEFT_SEL 33

// DC Motor SIPO Pins
#define SIPO_PIN_DC_MOTOR_TRACK_SEL 0
#define SIPO_PIN_DC_MOTOR_TRACK_INA 6
#define SIPO_PIN_DC_MOTOR_TRACK_INB 7

#define SIPO_PIN_DC_MOTOR_RIGHT_SEL 1
#define SIPO_PIN_DC_MOTOR_RIGHT_INA 4
#define SIPO_PIN_DC_MOTOR_RIGHT_INB 5

#define SIPO_PIN_DC_MOTOR_LEFT_INA 2
#define SIPO_PIN_DC_MOTOR_LEFT_INB 3

#endif //ESP32_ROBOTICA_CODE_PIN_DEFINITIONS_H
