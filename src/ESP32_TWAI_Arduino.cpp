#include "Arduino.h"
#include "ESP32_TWAI_Arduino.h"

// constructor
ESP32TWAI::ESP32TWAI(int crxPin, int ctxPin, long baudrate, int mode)
{
    _crxPin = crxPin;
    _ctxPin = ctxPin;
    _g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)_ctxPin, (gpio_num_t)_crxPin, (twai_mode_t)mode);
    _f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    setBauadrate(baudrate);
}

// destructor
ESP32TWAI::~ESP32TWAI()
{
    end();
}

int ESP32TWAI::begin()
{   
    //install driver
    int installState = twai_driver_install(&_g_config, &_t_config, &_f_config);
    if(installState != ESP_OK){
        return installState;
    }
    //start driver
    int startState = twai_start();
    if(startState != ESP_OK){
        return startState;
    }
    return 1;    
}

int ESP32TWAI::end()
{
    int uninstallState = twai_driver_uninstall();
    if(uninstallState != ESP_OK){
        return uninstallState;
    }
    int stopState = twai_start();
    if(stopState != ESP_OK){
        return stopState;
    }  
}

void ESP32TWAI::setPins(int crxPin, int ctxPin)
{
    _g_config.rx_io = (gpio_num_t)crxPin;
    _g_config.tx_io = (gpio_num_t)ctxPin;
}

void ESP32TWAI::setMode(int mode)
{
    switch (mode)
    {
    // Normal mode, send and receive, ack needed
    case TWAI_MODE_NORMAL:
        _g_config.mode = TWAI_MODE_NORMAL;
        break;
    // no  ack needed for transmission, ack provided on  receive
    case TWAI_MODE_NO_ACK:
        _g_config.mode = TWAI_MODE_NO_ACK;
        break;
    // no ack, no send
    case TWAI_MODE_LISTEN_ONLY:
        _g_config.mode = TWAI_MODE_LISTEN_ONLY;
        break;
    //
    default:
        _g_config.mode = TWAI_MODE_NORMAL;
        break;
    }
}

void ESP32TWAI::setBauadrate(long baudrate)
{
    switch (baudrate)
    {
#if (SOC_TWAI_BRP_MAX > 256)
    case 1000:
        _t_config = TWAI_TIMING_CONFIG_1KBITS();
        break;
    case 5000:
        _t_config = TWAI_TIMING_CONFIG_5KBITS();
        break;
    case 10000:
        _t_config = TWAI_TIMING_CONFIG_10KBITS();
        break;
#endif

// Chip revision above 2 needed
#if (SOC_TWAI_BRP_MAX > 128) || (CONFIG_ESP32_REV_MIN >= 2)
    case 12500:
        _t_config = TWAI_TIMING_CONFIG_12_5KBITS();
        break;
    case 16000:
        _t_config = TWAI_TIMING_CONFIG_16KBITS();
        break;
    case 20000:
        _t_config = TWAI_TIMING_CONFIG_20KBITS();
        break;
#endif

    case 25000:
        _t_config = TWAI_TIMING_CONFIG_25KBITS();
        break;
    case 50000:
        _t_config = TWAI_TIMING_CONFIG_50KBITS();
        break;
    case 100000:
        _t_config = TWAI_TIMING_CONFIG_100KBITS();
        break;
    case 125000:
        _t_config = TWAI_TIMING_CONFIG_125KBITS();
        break;
    case 250000:
        _t_config = TWAI_TIMING_CONFIG_250KBITS();
        break;
    case 500000:
        _t_config = TWAI_TIMING_CONFIG_500KBITS();
        break;
    case 800000:
        _t_config = TWAI_TIMING_CONFIG_800KBITS();
        break;
    case 1000000:
        _t_config = TWAI_TIMING_CONFIG_1MBITS();
        break;
    default:
        _t_config = TWAI_TIMING_CONFIG_500KBITS();
    }
}

int ESP32TWAI::availableMessages()
{
    if(twai_get_status_info(&_status_info) == ESP_OK){

        return _status_info.msgs_to_rx;
    }
    else{
        return -1;
    }
    
}

int ESP32TWAI::receiveMessage()
{
    if (twai_receive(&rxMessage, 1) == ESP_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void ESP32TWAI::dumpConfig()
{
    Serial.println(_g_config.mode);
    Serial.println(_t_config.brp);
    Serial.println(_f_config.acceptance_code);
}
