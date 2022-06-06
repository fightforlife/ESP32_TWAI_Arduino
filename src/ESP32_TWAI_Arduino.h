/*
  ESP32_SJA1000_CAN_Arduino.h - Library using the ESP32 internal SJA1000
  Created by https://github.com/fightforlife/ESP32_SJA1000_CAN_Arduino, 06. June 2022.
  Released into the public domain.
*/


#include "driver/gpio.h"
#include "driver/twai.h"
#include "hal/twai_hal.h"


#ifndef ESP32_TWAI_Arduino_h
#define ESP32_TWAI_Arduino_h


#define DEFAULT_CRX GPIO_NUM_4
#define DEFAULT_CTX GPIO_NUM_5
#define DEFAULT_BAUDRATE 500000
#define DEFAULT_MODE TWAI_MODE_NORMAL


class ESP32TWAI
{
  public:

    twai_message_t rxMessage;
    twai_message_t txMessage;

    ESP32TWAI(int crxPin = DEFAULT_CRX, int ctxPin = DEFAULT_CTX, long baudrate = DEFAULT_BAUDRATE, int mode = DEFAULT_MODE);
    ~ESP32TWAI();
    void setPins(int crxPin, int ctxPin);
    void setMode(int mode);
    void setBauadrate(long baudrate);
    int begin();
    int end();
    int availableMessages();
    int receiveMessage();
    void dumpConfig();





  private:
    int _crxPin;
    int _ctxPin;
    twai_timing_config_t _t_config;
    twai_general_config_t _g_config;
    twai_filter_config_t _f_config;
    twai_status_info_t _status_info;
    
    
};

#endif