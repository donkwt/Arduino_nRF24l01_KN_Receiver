// RF24KNReceiver.h

#ifndef _RF24KNRECEIVER_h
#define _RF24KNRECEIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <SPI.h>

//#include <RF24.h>			// Arduino UNO
//#define CE_PIN 7
//#define CS_PIN 8

#include <RF24_STM32.h>		// STM32duino
#define CE_PIN PA1
#define CS_PIN PA4


class RF24KNReceiver {
  private:
    uint8_t binding_addr[5];                  // binding address "KNDZK"
    uint8_t operation_addr[5];                // operational address
    uint8_t binding_channel = 83;             // 2,483 GHz
    uint8_t channels[4] = { 8, 24, 45, 65 };  // 2,408 2,424 2,445 2,465 GHz
    uint8_t current_channel = 0;
    uint8_t packet_number = 0;
    uint8_t buffer[16];
    int32_t tries_receive = 0;
    uint32_t tries_max = 1000;

  public:
    uint16_t left_x = 511;
    uint16_t left_y = 511;
    uint16_t right_x = 511;
    uint16_t right_y = 511;
    uint8_t left_x_trim = 100;
    uint8_t left_y_trim = 100;
    uint8_t right_x_trim = 100;
    uint8_t right_y_trim = 100;
    bool switch_throttle_hold = true;
    bool switch_idle = true;
    bool switch_dual_rate = false;
    bool switch_3d_6g = false;
    bool switch_mode = false;
    bool switch_hover_debugging = false;

    void init();
    bool bind();
    bool receive();
    bool available();
};


#endif

