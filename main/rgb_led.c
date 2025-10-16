/**
 * rgb_led.c
 *
 * @brief Source file for RGB LED control functions.
 * @details This file contains functions for controlling an RGB LED.
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */

#include <stdbool.h>
#include "driver/ledc.h"
#include "rgb_led.h"
#include "freertos/FreeRTOS.h"
led_info_t ledc_ch[RGB_LED_CHANNEL_NUM];

//handle for rgb_led_pwm_init
bool g_pwm_init_handle = false;

/**
 *Initialize the RGB LED settings per channel,
 *including the GPIO for each color, mode and timer configuration.
 */
static void rgb_led_pwm_init(void)
{
    int rgb_channel;

    // Red
    ledc_ch[0].channel      = LEDC_CHANNEL_0;
    ledc_ch[0].gpio         = RGB_LED_PIN_RED;
    ledc_ch[0].mode         = LEDC_HIGH_SPEED_MODE;
    ledc_ch[0].timer_index  = LEDC_TIMER_0;

    // Green
    ledc_ch[1].channel      = LEDC_CHANNEL_1;
    ledc_ch[1].gpio         = RGB_LED_PIN_GREEN;
    ledc_ch[1].mode         = LEDC_HIGH_SPEED_MODE;
    ledc_ch[1].timer_index  = LEDC_TIMER_0;

    // Blue
    ledc_ch[2].channel      = LEDC_CHANNEL_2;
    ledc_ch[2].gpio         = RGB_LED_PIN_BLUE;
    ledc_ch[2].mode         = LEDC_HIGH_SPEED_MODE;
    ledc_ch[2].timer_index  = LEDC_TIMER_0;

    //Configuration timer zero
    ledc_timer_config_t ledc_timer =
    {
        .duty_resolution = LEDC_TIMER_8_BIT,    // resolution of PWM duty
        .freq_hz = 5000,                        // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,     // timer mode
        .timer_num = LEDC_TIMER_0,              // timer index
    };

    ledc_timer_config(&ledc_timer);

    // Configure each channel of the RGB LED
    for (rgb_channel = 0; rgb_channel < RGB_LED_CHANNEL_NUM; rgb_channel++) {
        ledc_channel_config_t ledc_channel =
        {
            .channel    = ledc_ch[rgb_channel].channel,
            .duty       = 0,
            .gpio_num   = ledc_ch[rgb_channel].gpio,
            .speed_mode = ledc_ch[rgb_channel].mode,
            .hpoint     = 0,
            .timer_sel  = ledc_ch[rgb_channel].timer_index,
            .intr_type  = LEDC_INTR_DISABLE
        };
        ledc_channel_config(&ledc_channel);
    }

    g_pwm_init_handle = true;
}


/**
 * Sets the RGB Color.
 */
static void rgb_led_set_color(uint8_t red, uint8_t green, uint8_t blue)
{
    //Value should be between 0-255
    ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel, red);
    ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);
    
    ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel, green);
    ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);
        
    ledc_set_duty(ledc_ch[2].mode, ledc_ch[2].channel, blue);
    ledc_update_duty(ledc_ch[2].mode, ledc_ch[2].channel);

}

void rgb_led_wifi_app_started(void)
{
    if (!g_pwm_init_handle) {
        rgb_led_pwm_init();
    }

    const int delay_ms = 10;

    for (int i = 0; i < 255; i++) {
        uint8_t red = (uint8_t)((255 * i) / 255);
        uint8_t green = (uint8_t)((60 * i) / 255);
        uint8_t blue = 0;

        rgb_led_set_color(red, green, blue);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    for (int i = 255; i >= 0; i--) {
        uint8_t red = (uint8_t)((255 * i) / 255);
        uint8_t green = (uint8_t)((60 * i) / 255);
        uint8_t blue = 0;

        rgb_led_set_color(red, green, blue);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void rgb_led_http_server_started(void)
{
    if(g_pwm_init_handle == false)
    {
        rgb_led_pwm_init();
    }

    const int delay_ms = 10;

    for (int i = 0; i < 255; i++) {
        uint8_t red = 0;
        uint8_t green = (uint8_t)((100 * i) / 255);
        uint8_t blue  = (uint8_t)((255* i) / 255);;

        rgb_led_set_color(red, green, blue);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    for (int i = 255; i >= 0; i--) {
        uint8_t red = 0;
        uint8_t green = (uint8_t)((100 * i) / 255);
        uint8_t blue  = (uint8_t)((255* i) / 255);;

        rgb_led_set_color(red, green, blue);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void rgb_led_wifi_connected(void)
{
    if(g_pwm_init_handle == false)
    {
        rgb_led_pwm_init();
    }
        const int delay_ms = 10;

    for (int i = 0; i < 255; i++) {
        uint8_t red = 0;
        uint8_t blue  = 0;

        rgb_led_set_color(red, i, blue);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    for (int i = 255; i >= 0; i--) {
        uint8_t red = 0;
        uint8_t blue  = 0;

        rgb_led_set_color(red, i, blue);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}
