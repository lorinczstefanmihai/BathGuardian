/*
 * http_server.h
 *
 * @brief Source file for HTTP server control functions.
 * @details This file contains functions for controlling the HTTP server.
 * @created on: 2025-10-17
 * @author Lorincz Stefan
 */

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "freertos/FreeRTOS.h"

#define OTA_UPDATE_PENDING 		0
#define OTA_UPDATE_SUCCESSFUL	1
#define OTA_UPDATE_FAILED	   -1

/**
 * Connection status for Wifi
 */
typedef enum http_server_wifi_connect_status
{
	NONE = 0,
	HTTP_WIFI_STATUS_CONNECTING,
	HTTP_WIFI_STATUS_CONNECT_FAILED,
	HTTP_WIFI_STATUS_CONNECT_SUCCESS,
    HTTP_WIFI_STATUS_DISCONNECTED,
} http_server_wifi_connect_status_e;

/**
 * Messages for HTTP monitor
 */
typedef enum {
    HTTP_MSG_WIFI_CONNECT_INIT = 0,
    HTTP_MSG_WIFI_CONNNECT_SUCCESS,
    HTTP_MSG_WIFI_CONNECT_FAIL,
    HTTP_MSG_WIFI_USER_DISCONNECT,
    HTTP_MSG_OTA_UPDATE_SUCCESSFUL,
    HTTP_MSG_OTA_UPDATE_FAILED,
} http_server_msg_e;

/**
 * Structure for the message queue
 */
typedef struct http_server_queue_message {
    http_server_msg_e msg_id;
} http_server_queue_message_t;

/**
 * Sends a message to the queue
 * @param msg message ID from the hrrp_server_messsage_e enum
 * @return pdTRUE if the message was sent successfully, pdFALSE otherwise
 * @note Expand the parameter list based on your requirements e.g. how you've expanded the http_server_queue_message_t
 */
BaseType_t http_server_monitor_send_message(http_server_msg_e msg);

/**
 * Starts the HTTP server
 */
void http_server_start(void);

/**
 * Stops the HTTP server
 */
void http_server_stop(void);

/**
 * Timer callback function which calls esp_restart upon successful firmware update.
 */
void http_server_fw_update_reset_callback(void *arg);


 #endif // HTTP_SERVER_H

 