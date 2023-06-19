//
// Created by Klaas Jan on 29-5-2023.
//

#include "PowerController.hpp"
#include "SBC_Com.hpp"
#include "Lights.hpp"
#include "Motor_functions.hpp"

float_t bat_voltage = 0.0;
float_t bat_current = 0.0;
float_t i_incoming_voltage = 0.0;

void powerController() {
    // turn on the main power with undervoltage protection, monitor voltages and current
    //digitalWrite(ESP32_PIN_EN_MAIN_SW, HIGH);
//    char debug_str[100] = "";
//    char topic_str[50] = "Power controller";
//    sprintf(debug_str, "Voltage: %f\tCurrent: %f\tPin_shut_req = %d", get_bat_voltage(), get_bat_current(), digitalRead(ESP32_PIN_SHUT_REQ));
//    send_debug_data(topic_str, debug_str);

    if (digitalRead(ESP32_PIN_SHUT_REQ) == LOW) { //shutdown request button pressed?
        shutdown_SBC(); //shutdown SBC (Single Board Computer)
        //delay(2000);
        digitalWrite(ESP32_PIN_EN_MAIN_SW, LOW); //Turn main switch off
        //char debug_str3[100] = "Main switch off";
        //send_debug_data(topic_str, debug_str3);
    }

    else if(get_bat_voltage() > 9.9) { // if voltage is above 9.9V then keep on the main switch
        digitalWrite(ESP32_PIN_SHUT_REQ_SBC, HIGH);
        digitalWrite(ESP32_PIN_EN_MAIN_SW, HIGH); // Turn on main switch
//        char debug_str1[100] = "Main switch on";
//        send_debug_data(topic_str, debug_str1);
    }
    else { // if the battery voltage is low, turn of the system
        shutdown_SBC(); // turn of SBC before cutting of power
//        char debug_str2[100] = "Main switch off (else statement)";
//        send_debug_data(topic_str, debug_str2);
        digitalWrite(ESP32_PIN_EN_MAIN_SW, LOW); //turn of main switch

    }
}

void start_SBC() {
    //Not used as SBC turns on automatically
    delay(500); //wait half a second so that power supply is stable
    digitalWrite(ESP32_PIN_START_SHUT_SBC, LOW); //pull line low to start SBC for half a second
    delay(500);
    digitalWrite(ESP32_PIN_START_SHUT_SBC, HIGH); //leave pin high, while running SBC(Single Board Computer)
}


void shutdown_SBC() {
    // this function enables the SBC (Single board computer) to shutdown before the power is cuttoff
    simple_right_DC_motor_control(0);        //turn right motors
    simple_left_DC_motor_control(0);         //turn off left motors
    simple_track_DC_motor_control(0);        //turn off track motor
    display_matrix(Display_off);                //turn off matrix display
    display_bat_low();                              // indicate low battery voltage

    digitalWrite(ESP32_PIN_SHUT_REQ_SBC, LOW);

    delay(20000);   //wait for 20 seconds so that the SBC can shutdown
}

float_t get_bat_voltage() {
        bat_voltage = (float_t) 4.0*1.03*(3.3/4096.0) * (float_t) analogRead(ESP32_PIN_BAT_MEAS_VOLTAGE); // convert ADC reading to voltage
    return bat_voltage;
}


float_t get_bat_current() {
    i_incoming_voltage = (float_t) ((2.5/1.5)*(3.3/4096.0) * (float_t) (analogRead(ESP32_PIN_BAT_MEAS_CURRENT))); // convert ADC reading to voltage
    bat_current = (float_t) ((1.0/0.066) * (i_incoming_voltage-2.5)); // convert voltage reading to current
    return bat_current;
}

