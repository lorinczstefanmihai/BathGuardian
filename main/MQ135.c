/**
 * MQ135.c
 *
 * @brief Source file for MQ-135 sensor.
 * @details Provides analog read, voltage conversion, and periodic logging
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */


#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include "esp_log.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "MQ135.h"

static const char *TAG = "MQ135";

static adc1_channel_t mq135_channel = MQ135_DEFAULT_ADC_CHANNEL;
static esp_adc_cal_characteristics_t adc_chars;
static float raw = 0;
static float volt = 0;

// == Getter functions =======================================================

float getMQ135raw(void)     { return raw; }
float getMQ135volt(void)    { return volt; }

// ---------------- Configure ADC channel ----------------
void setMQ135Channel(adc1_channel_t channel)
{
    mq135_channel = channel;
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(mq135_channel, ADC_ATTEN_DB_11);

    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
}

// ---------------- Read raw ADC ------------------------
float readMQ135Raw(void)
{
    int adc_raw = adc1_get_raw(mq135_channel);
    return (float)adc_raw;
}

// ---------------- Read voltage (mV) ------------------
float readMQ135Voltage(void)
{
    int adc_raw = adc1_get_raw(mq135_channel);
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_raw, &adc_chars);
    return (float)voltage / 1000.0f; // return volts
}

// ---------------- FreeRTOS task ----------------------
static void MQ135_task(void *pvParameter)
{
    setMQ135Channel(mq135_channel);

    for (;;)
    {
        raw = readMQ135Raw();
        volt = readMQ135Voltage();
        ESP_LOGI(TAG, "Raw: %.0f | Voltage: %.2f V", raw, volt);
        vTaskDelay(pdMS_TO_TICKS(MQ135_READ_INTERVAL_MS));
    }
}

void MQ135_task_start(void)
{
    xTaskCreatePinnedToCore(&MQ135_task, "MQ135_task",
                            MQ135_TASK_STACK_SIZE, NULL,
                            MQ135_TASK_PRIORITY, NULL,
                            MQ135_TASK_CORE_ID);
}
