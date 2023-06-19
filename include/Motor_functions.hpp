//
// Created by Klaas Jan on 29-5-2023.
//

#ifndef ESP32_ROBOTICA_CODE_MOTOR_FUNCTIONS_HPP
#define ESP32_ROBOTICA_CODE_MOTOR_FUNCTIONS_HPP

#include <Arduino.h>
#include "Pin_setup.hpp"
#include "SBC_Com.hpp"
#include <string.h>
#include <stdio.h>

#define PWM0_Ch    0
#define PWM0_Res   8
#define PWM0_Freq  1000

#define PWM1_Ch    1
#define PWM1_Res   8
#define PWM1_Freq  1000

#define PWM2_Ch    2
#define PWM2_Res   8
#define PWM2_Freq  1000

//#define DEBUG_MODE

//defines the states of the motor drive/motors
typedef enum modes {AtoB, BtoA, Brake, Off} Motor_controller_modes;

//defines the motorcontroller pins
typedef struct motor_controller_pins {
    boolean inA;
    boolean inB;
    boolean sel;
    uint8_t pwm;
} DC_motor_controller_pins;


void init_DC_motor_controller();

DC_motor_controller_pins control_DC_motor(Motor_controller_modes mode, uint8_t speed);
DC_motor_controller_pins simple_DC_motor_control(int16_t speed);

void simple_left_DC_motor_control(int16_t speed);
void simple_right_DC_motor_control(int16_t speed);
void simple_track_DC_motor_control(int16_t speed);
void test_dc_motors();

uint8_t change_bit(uint8_t byte, boolean value, uint8_t index);
void update_register(uint8_t data);

#endif
