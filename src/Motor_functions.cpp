#include "Motor_functions.hpp"

uint8_t shift_data = 0; //variable used to send data to shiftregister

void init_DC_motor_controller(){ //Sets up the microcontroller for the DC-motor controllers
    //set up al the PWM channels and their multiplexers
    ledcAttachPin(ESP32_PIN_DC_MOTOR_LEFT_PWM, PWM0_Ch);
    ledcSetup(PWM0_Ch, PWM0_Freq, PWM0_Res);
    ledcAttachPin(ESP32_PIN_DC_MOTOR_RIGHT_PWM, PWM1_Ch);
    ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
    ledcAttachPin(ESP32_PIN_DC_MOTOR_TRACK_PWM, PWM2_Ch);
    ledcSetup(PWM2_Ch, PWM2_Freq, PWM2_Res);
}

//universal functions to give pin states of motor controller
DC_motor_controller_pins control_DC_motor(Motor_controller_modes mode, uint8_t speed) {
    DC_motor_controller_pins motor_pins;

    switch (mode) {
        case AtoB:
            //sets pins to a mode that current flows from OUTA to OUTB from motor driver
            motor_pins.inA = true;
            motor_pins.inB = false;
            motor_pins.sel = false;
            motor_pins.pwm = speed;
            break;

        case BtoA:
            //sets pins to a mode that current flows from OUTB to OUTA from motor driver
            motor_pins.inA = false;
            motor_pins.inB = true;
            motor_pins.sel = true;
            motor_pins.pwm = speed;
            break;

        case Brake:
            //sets pins from motor driver to short the OUTA and OUTB terminals from the motor driver
            motor_pins.inA = false;
            motor_pins.inB = false;
            motor_pins.pwm = 255;
            break;

        case Off:
            //turns off motorcontroller motors wont be powered or shorted.
            motor_pins.inA = false;
            motor_pins.inB = false;
            motor_pins.pwm = false;
            break;
    }
    return motor_pins;
}

//universal function that gives pin states using control_DC_motor() function.
// Also does conversions to make control simpler.
DC_motor_controller_pins simple_DC_motor_control(int16_t speed) {
    DC_motor_controller_pins dc_motor_pin;
    Motor_controller_modes dc_motor_mode;
    uint8_t pwm_speed = 0;

    if (speed == 0) { //if speed = 0 set the motor in brake mode
        dc_motor_mode = Brake;
    }
    else if (speed > 0 && speed <= 255) { //current wil flow from OUTA to OUTB terminal on motor driver
        dc_motor_mode = AtoB;
        pwm_speed = (uint8_t) abs(speed);
    }
    else if (speed < 0 && speed >= -255) {
        dc_motor_mode = BtoA;   //current wil flow from OUTB to OUTA terminal on motor driver
        pwm_speed = (uint8_t) abs(speed); //set dutyCycle of motors
    }
    else {
        dc_motor_mode = Off; //turn off driver
    }
    dc_motor_pin = control_DC_motor(dc_motor_mode, pwm_speed); //control_DC_motor() wil set al the neccesary functions
    return dc_motor_pin;
}


//hardware interface for left DC motor driver
void simple_left_DC_motor_control(int16_t speed) {
    DC_motor_controller_pins left_dc_motor;
    left_dc_motor = simple_DC_motor_control(speed);     //decide pin values of motorcontroller

    //set pins which are are controlled by a shift register.
    shift_data = change_bit(shift_data, left_dc_motor.inA, SIPO_PIN_DC_MOTOR_LEFT_INA);
    shift_data = change_bit(shift_data, left_dc_motor.inB, SIPO_PIN_DC_MOTOR_LEFT_INB);
    update_register(shift_data);

    //set pins which are are controlled by a GPIO pins.
    digitalWrite(ESP32_PIN_DC_MOTOR_LEFT_SEL, left_dc_motor.sel);
    ledcWrite(PWM0_Ch, left_dc_motor.pwm);    //set Duty Cycle on PWM pin
};


//hardware interface for right DC motor driver
void simple_right_DC_motor_control(int16_t speed) {
    DC_motor_controller_pins right_dc_motor;
    right_dc_motor = simple_DC_motor_control(speed); //decide pin values of motorcontroller

    //set pins which are are controlled by a shift register.
    shift_data = change_bit(shift_data, right_dc_motor.inA, SIPO_PIN_DC_MOTOR_RIGHT_INA);
    shift_data = change_bit(shift_data, right_dc_motor.inB, SIPO_PIN_DC_MOTOR_RIGHT_INB);
    shift_data = change_bit(shift_data, right_dc_motor.sel, SIPO_PIN_DC_MOTOR_RIGHT_SEL);
    update_register(shift_data);

    //set pins which are are controlled by a GPIO pins.
    ledcWrite(PWM1_Ch, right_dc_motor.pwm);
};


//hardware interface for track DC motor driver
void simple_track_DC_motor_control(int16_t speed) {
    DC_motor_controller_pins track_dc_motor;
    track_dc_motor = simple_DC_motor_control(speed); //decide pin values of motorcontroller

    //set pins which are are controlled by a shift register.
    shift_data = change_bit(shift_data, track_dc_motor.inA, SIPO_PIN_DC_MOTOR_TRACK_INA);
    shift_data = change_bit(shift_data, track_dc_motor.inB, SIPO_PIN_DC_MOTOR_TRACK_INB);
    shift_data = change_bit(shift_data, track_dc_motor.inB, SIPO_PIN_DC_MOTOR_TRACK_SEL);
    update_register(shift_data);

    //set pins which are are controlled by a GPIO pins.
    ledcWrite(PWM2_Ch, track_dc_motor.pwm);
};


//test script for DC motors
void test_dc_motors() {
    for (int16_t i = 0; i < 255; i++) {
        simple_left_DC_motor_control(i);
        simple_right_DC_motor_control(i);
        simple_track_DC_motor_control(i);
        delay(10);
    }
    delay(1000);

    for (int16_t i = 255; i > 0; i--) {
        simple_left_DC_motor_control(i);
        simple_right_DC_motor_control(i);
        simple_track_DC_motor_control(i);
        delay(10);
    }
    delay(1000);

    for (int16_t i = 0; i > -255; i--) {
        simple_left_DC_motor_control(i);
        simple_right_DC_motor_control(i);
        simple_track_DC_motor_control(i);
        delay(10);
    }
    delay(1000);


    for (int16_t i = -255; i < 0; i++) {
        simple_left_DC_motor_control(i);
        simple_right_DC_motor_control(i);
        simple_track_DC_motor_control(i);
        delay(10);
    }
    delay(1000);
}


uint8_t change_bit(uint8_t byte, boolean value, uint8_t index) {
//    char debug_str[100] = "";
//    char topic_str[50] = "Change bit";
//    sprintf(debug_str, "[change bit] begin byte = 0x%X\tvalue = 0x%d\t index = %d", byte, value, index);
//    send_debug_data(topic_str, debug_str);
    uint8_t mask = B10000000 >> index; //creates a mask to clear value at index
//    sprintf(debug_str, "mask byte is = 0x%X", byte);
//    send_debug_data(topic_str, debug_str);

    byte &= ~mask; //clears bit at given index
//    sprintf(debug_str, "cleared byte is = 0x%X", byte);
//    send_debug_data(topic_str, debug_str);
    byte |= value << (7-index); //sets bit at index to same as value
    //byte = B00001000;

//    sprintf(debug_str, "changed byte is = 0x%X", byte);
//    send_debug_data(topic_str, debug_str);
    return byte;
}


void update_register(uint8_t data) { //function to write data to shiftregister (74HC595N)
    // for more information about sending data to the shift register, visit https://lastminuteengineers.com/74hc595-shift-register-arduino-tutorial/
    //data format B[ABCDEFGH]
    digitalWrite(ESP32_PIN_SIPO_RCLK, LOW); //sets latch pin low
    shiftOut(ESP32_PIN_SIPO_DATA, ESP32_PIN_SIPO_SRCLK, LSBFIRST, data); // writes data to shift register in a spi like fashion
    digitalWrite(ESP32_PIN_SIPO_RCLK, HIGH);    //sets the latch pin high, to load the shifted values in the storage register.
}

