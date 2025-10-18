/**
 * Application main entry point.
 */

 #include "nvs_flash.h"
 #include "wifi_app.h"
 #include "DHT11.h"
 #include "MQ135.h"

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init(); 
    }

    ESP_ERROR_CHECK(ret);

    //Start the WiFi application
    wifi_app_task_start();

    //Start the DHT11 sensor task
    DHT11_task_start();

    //Start the MQ135 sensor task
    MQ135_task_start();
}