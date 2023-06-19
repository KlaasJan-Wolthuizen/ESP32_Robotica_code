#include <Arduino.h>
#include "Pin_setup.hpp"
#include "PowerController.hpp"
#include "Motor_functions.hpp"
#include "Lights.hpp"
#include "SBC_Com.hpp"

//#define DEBUG_MODE

//TaskHandle_t Core1;
//TaskHandle_t Core2;

//void core1Code(void * pvParameters);
//void core2Code(void * pvParameters);


display_mode mode_SBC;

void setup() {
    // write your initialization code here
    ESP32_pin_setup(); //sets up the pins of the microcontroller
    init_DC_motor_controller(); //Sets up the microcontroller for the DC-motor controllers
    init_lights(); //sets up the matrix, and initializes it.
    init_serial(); //sets up serial communication for the SBC

    powerController(); // turn on the main power with undervoltage protection, monitor voltages and current

//    xTaskCreatePinnedToCore(
//            core1Code,   /* Task function. */
//            "Core1",     /* name of task. */
//            10000,       /* Stack size of task */
//            NULL,        /* parameter of the task */-
//            1,           /* priority of the task */
//            NULL,      /* Task handle to keep track of created task */
//            0);          /* pin task to core 0 */

//    xTaskCreatePinnedToCore(
//            core2Code,   /* Task function. */
//            "Core2",     /* name of task. */
//            10000,       /* Stack size of task */
//            NULL,        /* parameter of the task */
//            1,           /* priority of the task */
//            NULL,      /* Task handle to keep track of created task */
//            0);          /* pin task to core 0 */
}

//void core1Code(void * pvParameters) {
//    display_mode(VU);
//}


void loop() {
    powerController(); // turn on the main power with undervoltage protection, monitor voltages and current
    comm_SBC();        // Communicates with SBC (Single board computer) and takes the corresponding actions.
    display_matrix(mode_SBC);   //display matrix in given mode
}
