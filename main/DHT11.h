/**
 * DHT11.h
 *
 * @brief Header file for  DHT11 sensor.
 * @details Provides analog read, voltage conversion, and periodic logging
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */
#ifndef __DHT11_H__
#define __DHT11_H__

#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ---- Constants ------------------------------------------------------------

#define DHT_OK                0
#define DHT_CHECKSUM_ERROR   -1
#define DHT_TIMEOUT_ERROR    -2

// ---- Configuration --------------------------------------------------------

#define DHT11_GPIO            4   // Default GPIO pin (can be changed)
#define DHT11_TASK_STACK_SIZE 2048
#define DHT11_TASK_PRIORITY   5
#define DHT11_TASK_CORE_ID    1

// ---- Public API -----------------------------------------------------------

void setDHT11gpio(int gpio);

float getDHT11Humidity(void);
float getDHT11Temperature(void);

int readDHT11(void);

void errorHandler(int response);

// Optional FreeRTOS Task launcher
void DHT11_task_start(void);

#endif /* __DHT11_H__ */
