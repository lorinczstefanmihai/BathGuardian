/*
 * rgb_led.h
 *
 * @brief Source file for RGB LED control functions.
 * @details This file contains functions for controlling an RGB LED.
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */

#ifndef RGB_LED_H
#define RGB_LED_H

// Add your function declarations and macros here
#define RGB_LED_PIN_RED    18  // GPIO pin for Red LED
#define RGB_LED_PIN_GREEN  17  // GPIO pin for Green LED
#define RGB_LED_PIN_BLUE   19  // GPIO pin for Blue LED 

//RGB LED color mix channels
#define RGB_LED_CHANNEL_NUM 3

//RGB LED configuration structure
typedef struct {
    int channel; 
    int gpio;    
    int mode;    
    int timer_index; 
} led_info_t;

/**
 *  Color to indicate Wifi application has started.
 */
void rgb_led_wifi_app_started(void);

/**
 * Color to indicate HTTP server is started and ready to accept connections.
 */
void rgb_led_http_server_started(void);

/**
 * Color to indicate that the ESP32 is coonected to access point.
 */
void rgb_led_wifi_connected(void);

#endif // RGB_LED_H