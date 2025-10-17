/**
 * wifi_app.c
 *
 * @brief WiFi application source file for BathGuardian project.
 * @details This file contains the implementation of WiFi-related functionality.
 * @created on: 2025-10-14
 * @author Lorincz Stefan
 */

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "lwip/netbuf.h"
#include <arpa/inet.h>
#include <sys/socket.h>


#include "rgb_led.h"
#include "tasks_common.h"
#include "wifi_app.h"


// Tag used for ESP serial console messages
static QueueHandle_t wifi_app_queue_handle;

static const char *TAG = "wifi_app";

// netif objects for the station and access point
esp_netif_t *wifi_sta_netif = NULL;
esp_netif_t *wifi_ap_netif = NULL;


/**
 * WIFi application event handler
 * @param arg data, aside from event data, that is passed to the handler when it is called
 * @param event_base the base ID of the event to register the handler for
 * @param event_id the ID of the event to register the handler for
 * @param event_data a pointer to data associated with the event
 */
static void wifi_app_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if(event_base == WIFI_EVENT)
    {
        switch(event_id)
        {
            case WIFI_EVENT_AP_START:
                ESP_LOGI(TAG, "WiFi AP started");
                break;

            case WIFI_EVENT_AP_STOP:
                ESP_LOGI(TAG, "WiFi AP stopped");
                break;

            case WIFI_EVENT_AP_STACONNECTED:
                ESP_LOGI(TAG, "WiFi AP station connected");
                break;

            case WIFI_EVENT_AP_STADISCONNECTED:
                ESP_LOGI(TAG, "WiFi AP station disconnected");
                break;

            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "WiFi STA started");
                break;

            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "WiFi STA connected");
                break;   

            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "WiFi STA disconnected");
                break;
            
        }
    }
    else if(event_base == IP_EVENT)
    {
        switch(event_id)
        {
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(TAG, "WiFi STA got IP address");
                //wifi_app_send_message(WIFI_APP_MSG_STA_CONNECTED_GOT_IP);
                break;
            case IP_EVENT_STA_LOST_IP:
                ESP_LOGI(TAG, "WiFi STA lost IP address");
                break;
            case IP_EVENT_AP_STAIPASSIGNED:
                ESP_LOGI(TAG, "AP assigned IP to station");
                break;
            case IP_EVENT_GOT_IP6:
                ESP_LOGI(TAG, "WiFi interface got IPv6 address");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled IP event: %d", event_id);
                break;
        }
    }
}

/**
 * Initializes the WiFi application event handler for WiFi and IP events.
 */
static void wifi_app_event_handler_init(void)
{
    //Event loop for the WiFi driver
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    //Event handler for the connection
    esp_event_handler_instance_t instance_wifi_event;  
    esp_event_handler_instance_t instance_ip_event;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_app_event_handler, NULL, &instance_wifi_event));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &wifi_app_event_handler, NULL, &instance_ip_event));

}

/**
 * Initiliazes the TCP Stack and default WiFi configuration
 */
static void wifi_app_init(void)
{
    //Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    //Create default WiFi station and access point
    wifi_sta_netif = esp_netif_create_default_wifi_sta();
    wifi_ap_netif = esp_netif_create_default_wifi_ap();

    //Initialize the WiFi driver with default configuration
    wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));

    //Set WiFi mode to AP+STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));

    //Set WiFi storage to RAM
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
}

/**
 *  Configures the WiFi Access Point settings and assing the static IP to the AP
 */
static void wifi_app_soft_ap_config(void)
{
    //SoftAP configuration structure
    wifi_config_t wifi_ap_config = {
        .ap = {
            .ssid = WIFI_AP_SSID,
            .ssid_len = strlen(WIFI_AP_SSID),
            .password = WIFI_AP_PASSWORD,
            .channel = WIFI_AP_CHANNEL,
            .authmode = strlen(WIFI_AP_PASSWORD) == 0 ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = WIFI_AP_SSID_HIDDEN,
            .max_connection = WIFI_AP_MAX_CONNECTIONS,
            .beacon_interval = WIFI_AP_BEACON_INTERVAL
        },
    };

    //Configure DHCP for the Access Point
    esp_netif_ip_info_t ap_ip_info;
    memset(&ap_ip_info, 0, sizeof(ap_ip_info));

    esp_netif_dhcps_stop(wifi_ap_netif);            //Stop DHCP server
    inet_pton(AF_INET, WIFI_AP_IP, &ap_ip_info.ip);    //Assign accesp point's static IP, GW, and netmask
    inet_pton(AF_INET, WIFI_AP_GATEWAY, &ap_ip_info.gw);
    inet_pton(AF_INET, WIFI_AP_NETMASK, &ap_ip_info.netmask);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(wifi_ap_netif, &ap_ip_info)); //Statically configure the network interface
    esp_netif_dhcps_start(wifi_ap_netif); //Start the AP DHCP server ( for connecting stations e.g. mobile devices )
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA)); //Set WiFi mode to Access Point / Station
    ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_AP, WIFI_AP_BANDWIDTH)); //Set the WiFi bandwidth for the Access Point
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config)); //Set the WiFi configuration for the Access Point
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_STA_POWER_SAVE_MODE)); //Set the power save mode for the Station
}

/**
 * Main task for the WiFi application
 * @param pvParameters parameter which can be passed to the task
 */
static void wifi_app_task(void *pvParameters)
{
    wifi_app_queue_message_t msg;

    // Initialize the event handler
    wifi_app_event_handler_init();

    // Initialize the TCP/IP Stack and WiFi config 
    wifi_app_init();

    //SoftAP config
    wifi_app_soft_ap_config();

    //Start WiFi
    ESP_ERROR_CHECK(esp_wifi_start());

    //Send first event message 
    wifi_app_send_message(WIFI_APP_MSG_START_HTTP_SERVER);

    while (1)
    {
        // Wait indefinitely for a message from the queue
        if (xQueueReceive(wifi_app_queue_handle, &msg, portMAX_DELAY))
        {
            switch (msg.msgID)
            {
                case WIFI_APP_MSG_START_HTTP_SERVER:
                    ESP_LOGI(TAG, "Starting HTTP server...");
                    // Start your HTTP server here
                    rgb_led_http_server_started();
                    break;

                case WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER:
                    ESP_LOGI(TAG, "Connecting to WiFi from HTTP server...");
                    // Handle connection request from HTTP server here
                    break;

                case WIFI_APP_MSG_STA_CONNECTED_GOT_IP:
                    ESP_LOGI(TAG, "Station connected and got IP address.");
                    // Handle post-connection tasks here
                    rgb_led_wifi_connected();
                    break;

                default:
                    ESP_LOGW(TAG, "Received unknown message ID: %d", msg.msgID);
                    break;
            }
        }
    }
}

BaseType_t wifi_app_send_message(wifi_app_message_e msgID)
{
    wifi_app_queue_message_t msg;
    msg.msgID = msgID;
    return xQueueSend(wifi_app_queue_handle, &msg, portMAX_DELAY);
}

// Add your WiFi application code below
void wifi_app_task_start(void)
{
    ESP_LOGI(TAG, "Starting WiFi application task...");


    //Start WiFi started LED indication
    rgb_led_wifi_app_started();

    //Disable default WiFi logging messages
    esp_log_level_set("wifi", ESP_LOG_NONE);

    //Create the message queue
    wifi_app_queue_handle = xQueueCreate(3, sizeof(wifi_app_queue_message_t));

    //Start the WiFi application task
    xTaskCreatePinnedToCore(&wifi_app_task, "wifi_app_task", WIFI_APP_TASK_STACK_SIZE, NULL, WIFI_APP_TASK_PRIORITY, NULL, WIFI_APP_CORE_ID);
}
