//
// Created by Klaas Jan on 29-5-2023.
//
#include "Pin_setup.hpp"

void ESP32_pin_setup(){ //sets up the pins of the microcontroller
    // Powersystem control and monitoring pins
    pinMode(ESP32_PIN_BAT_MEAS_CURRENT, INPUT);
    pinMode(ESP32_PIN_BAT_MEAS_VOLTAGE, INPUT);
    pinMode(ESP32_PIN_START_SHUT_SBC, OUTPUT_OPEN_DRAIN);
    pinMode(ESP32_PIN_SHUT_REQ_SBC, OUTPUT_OPEN_DRAIN);
    pinMode(ESP32_PIN_SHUT_REQ, INPUT);
    pinMode(ESP32_PIN_EN_MAIN_SW, OUTPUT);
    
    // LED Strip pins
    pinMode(ESP32_PIN_MATRIX_DATA, OUTPUT);
    pinMode(ESP32_PIN_STRIP1_DATA, OUTPUT);
    pinMode(ESP32_PIN_STRIP2_DATA, OUTPUT);
    
    //SIPO(Serial In Parallel Out) Shift register pins (74HC595N)
    pinMode(ESP32_PIN_SIPO_SRCLK, OUTPUT);      // Serial clock output pin
    pinMode(ESP32_PIN_SIPO_DATA, OUTPUT);       // Serial data input
    pinMode(ESP32_PIN_SIPO_RCLK, OUTPUT);       // Latch pin, tranfers shift register data to storage register
    
    // DC Motor ESP Pins
    pinMode(ESP32_PIN_DC_MOTOR_LEFT_PWM, OUTPUT);
    pinMode(ESP32_PIN_DC_MOTOR_RIGHT_PWM, OUTPUT);
    pinMode(ESP32_PIN_DC_MOTOR_TRACK_PWM, OUTPUT);
    pinMode(ESP32_PIN_DC_MOTOR_LEFT_SEL, OUTPUT);

    //builtin led
    pinMode(LED_BUILTIN, OUTPUT);
}
