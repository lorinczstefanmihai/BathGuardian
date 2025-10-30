/**
 * wifi_app.h
 *
 * @brief WiFi application header file.
 * @details This file contains function prototypes and definitions for initializing and starting the WiFi application.
 * @details This file contains functions for controlling an RGB LED.
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */


#ifndef WIFI_APP_H_
#define WIFI_APP_H_

// Add your includes here
#include <stdbool.h>
#include <esp_netif.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"

// Function prototypes
static void wifi_app_init(void);
void wifi_app_task_start(void);

#define WIFI_AP_SSID               "BathGuardian-AP"  //AP name
#define WIFI_AP_PASSWORD           "fresh123"         //AP password
#define WIFI_AP_CHANNEL            1                  //AP channel
#define WIFI_AP_SSID_HIDDEN        0                  //AP visibility
#define WIFI_AP_MAX_CONNECTIONS    4                  //Max max clients
#define WIFI_AP_BEACON_INTERVAL    100                //AP beacon interval in ms
#define WIFI_AP_IP                 "192.168.0.1"      //AP default IP address
#define WIFI_AP_GATEWAY            "192.168.0.1"      //AP default gateway ( should be the same as the IP )
#define WIFI_AP_NETMASK            "255.255.255.0"    //AP netmask
#define WIFI_AP_BANDWIDTH           WIFI_BW_HT20      //AP bandwidth ( options: WIFI_BW_HT20, WIFI_BW_HT40, WIFI_BW_HT80, WIFI_BW_HT160 mhz )
#define WIFI_STA_POWER_SAVE_MODE    WIFI_PS_NONE      //STA power save mode ( options: WIFI_PS_NONE, WIFI_PS_MIN_MODEM, WIFI_PS_MAX_MODEM, WIFI_PS_MAX_PSP )
#define MAX_SSID_LENGTH             32                //Max SSID length
#define MAX_PASSWORD_LENGTH         64                //Max password length
#define MAX_CONNECTION_RETRIES      5                 //Max connection retries

// netfi object for the Station and Access Point
extern esp_netif_t *wifi_sta_netif;
extern esp_netif_t *wifi_ap_netif;

/**
 * Message IDs for the WiFI application task
 * @note Expand this based on your application requirements
 */
 typedef enum wifi_app_message
 {
    WIFI_APP_MSG_START_HTTP_SERVER = 0,
    WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
    WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
    WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT,
    WIFI_APP_MSG_LOAD_SAVED_CREDIANTIALS,
    WIFI_APP_MSG_STA_DISCONNECTED
 }wifi_app_message_e;

 /**
  * Structure for the message queue
  * @note Expand this based on your application requirements
  */
 typedef struct wifi_app_queue_message
 {
    wifi_app_message_e msgID;   // Message ID
 }wifi_app_queue_message_t;

 /**
  * Sends a message to the queue
  * @param msgID message ID from the wifi app+message_e enum
  * @return pdTRUE if the message was sent successfully, otherwise pdFALSE
  * #note Expand this based on your application requirements
  */
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);

/**
 * Start the WiFi RTOS task
 */
void wifi_app_task_start(void);

/**
 * Gets the wifi configuration
 */
wifi_config_t* wifi_app_get_wifi_config(void);


#endif // WIFI_APP_H_