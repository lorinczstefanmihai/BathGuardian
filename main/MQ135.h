/**
 * MQ135.h
 *
 * @brief Header file for MQ-135 sensor.
 * @details Provides analog read, voltage conversion, and periodic logging
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */

#ifndef __MQ135_H__
#define __MQ135_H__

#include "esp_err.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// --- Configuration ---
#define MQ135_DEFAULT_ADC_CHANNEL    ADC1_CHANNEL_4   // GPIO32
#define MQ135_TASK_STACK_SIZE        2048
#define MQ135_TASK_PRIORITY          5
#define MQ135_TASK_CORE_ID           1
#define MQ135_READ_INTERVAL_MS       2000             // Default 2s

// --- Public API ---
float getMQ135raw(void);
float getMQ135volt(void);

void setMQ135Channel(adc1_channel_t channel);
float readMQ135Raw(void);            // Return raw ADC value
float readMQ135Voltage(void);        // Convert raw ADC to voltage
void MQ135_task_start(void);         // Start periodic reading task

#endif /* __MQ135_H__ */
