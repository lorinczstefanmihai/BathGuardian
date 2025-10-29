/*------------------------------------------------------------------------------
    DHT11 temperature & humidity sensor driver for ESP32

    Style and documentation adapted from Ricardo Timmermann’s DHT22 driver.
    Implements timing-based bit reading for DHT11 sensor.

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    PLEASE KEEP THIS CODE IN LESS THAN 0xFF LINES. EACH LINE MAY CONTAIN ONE BUG!
------------------------------------------------------------------------------*/

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include "DHT11.h"
#include "tasks_common.h"

// == Global defines ==========================================================

static const char *TAG = "DHT11";

static int DHTgpio = DHT11_GPIO;
static float humidity = 0.0;
static float temperature = 0.0;

// == GPIO configuration =====================================================

void setDHT11gpio(int gpio)
{
    DHTgpio = gpio;
}

// == Getter functions =======================================================

float getDHT11Humidity(void)    { return humidity; }
float getDHT11Temperature(void) { return temperature; }

// == Error handler ==========================================================

void errorHandler(int response)
{
    switch (response)
    {
    case DHT_TIMEOUT_ERROR:
        ESP_LOGE(TAG, "Sensor Timeout");
        break;

    case DHT_CHECKSUM_ERROR:
        ESP_LOGE(TAG, "Checksum Error");
        break;

    case DHT_OK:
        break;

    default:
        ESP_LOGE(TAG, "Unknown error");
        break;
    }
}

/*------------------------------------------------------------------------------
;
;   get next signal state duration
;   returns duration in microseconds or -1 on timeout
;
;----------------------------------------------------------------------------*/

static int getSignalLevel(int usTimeOut, bool state)
{
    int uSec = 0;
    while (gpio_get_level(DHTgpio) == state)
    {
        if (uSec > usTimeOut)
            return -1;

        ++uSec;
        esp_rom_delay_us(1);
    }
    return uSec;
}

/*------------------------------------------------------------------------------
;
;   read DHT11 sensor
;
;   DATA: Hum = 8 bits, Temp = 8 bits, Checksum = 8 bits
;
;   DHT11 timing (simplified from DHT22):
;   - MCU pulls line low for >18ms
;   - MCU pulls line high for 20–40µs
;   - DHT responds: 80µs low + 80µs high
;   - Then sends 40 bits (8*5 bytes)
;       bit '0' = 26–28 µs high
;       bit '1' = 70 µs high
;----------------------------------------------------------------------------*/

#define MAXdhtData 5

int readDHT11(void)
{
    uint8_t dhtData[MAXdhtData] = {0};
    uint8_t byteInx = 0;
    uint8_t bitInx = 7;
    int uSec = 0;

    // == Start signal =======================================================
    gpio_set_direction(DHTgpio, GPIO_MODE_OUTPUT);
    gpio_set_level(DHTgpio, 0);
    vTaskDelay(pdMS_TO_TICKS(20));  // >18 ms -> start pulse
    gpio_set_level(DHTgpio, 1);
    esp_rom_delay_us(30);
    gpio_set_direction(DHTgpio, GPIO_MODE_INPUT);

    // == DHT response =======================================================
    if ((uSec = getSignalLevel(85, 0)) < 0) return DHT_TIMEOUT_ERROR;
    if ((uSec = getSignalLevel(85, 1)) < 0) return DHT_TIMEOUT_ERROR;

    // == Read 40 bits =======================================================
    for (int k = 0; k < 40; k++)
    {
        if ((uSec = getSignalLevel(56, 0)) < 0) return DHT_TIMEOUT_ERROR;
        if ((uSec = getSignalLevel(75, 1)) < 0) return DHT_TIMEOUT_ERROR;

        if (uSec > 40)
            dhtData[byteInx] |= (1 << bitInx);

        if (bitInx == 0)
        {
            bitInx = 7;
            ++byteInx;
        }
        else
            bitInx--;
    }

    // == Extract data ======================================================
    humidity = dhtData[0];
    temperature = dhtData[2];

    // == Verify checksum ===================================================
    uint8_t sum = dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3];
    if (dhtData[4] != (sum & 0xFF)) // overflow protection, only stores the lowest 8 bits
        return DHT_CHECKSUM_ERROR;

    return DHT_OK;
}

/*------------------------------------------------------------------------------
;
;   FreeRTOS DHT11 task
;
;----------------------------------------------------------------------------*/

static void DHT11_task(void *pvParameter)
{
    setDHT11gpio(DHT11_GPIO);

    for (;;)
    {
        int ret = readDHT11();
        errorHandler(ret);

        if (ret == DHT_OK)
        {
            ESP_LOGI(TAG, "Humidity: %.1f%% | Temperature: %.1f°C",
                     getDHT11Humidity(), getDHT11Temperature());
        }

        vTaskDelay(4000 / portTICK_PERIOD_MS);; // >=2s required
    }
}

void DHT11_task_start(void)
{
    xTaskCreatePinnedToCore(&DHT11_task, "DHT11_task",
                            DHT11_TASK_STACK_SIZE, NULL,
                            DHT11_TASK_PRIORITY, NULL,
                            DHT11_TASK_CORE_ID);
}