/**
 * tasks_common.h
 *
 * @brief   Common definitions for tasks.
 * @details This file contains common definitions and declarations used across different tasks in the project. 
 * @created on: 2025-10-13
 * @author Lorincz Stefan
 */


#ifndef TASKS_COMMON_H
#define TASKS_COMMON_H

// Common definitions for tasks
#define WIFI_APP_TASK_STACK_SIZE    4096
#define WIFI_APP_TASK_PRIORITY      5
#define WIFI_APP_CORE_ID            0

//HTTP server task 
#define HTTP_SERVER_TASK_STACK_SIZE 8192
#define HTTP_SERVER_TASK_PRIORITY   4 //(one priority level below WiFi task)
#define HTTP_SERVER_CORE_ID         0

//HTTP Server Monitor task
#define HTTP_SERVER_MONITOR_TASK_STACK_SIZE 4096
#define HTTP_SERVER_MONITOR_TASK_PRIORITY   3
#define HTTP_SERVER_MONITOR_CORE_ID         0

// DHT11 Sensor task
#define DHT11_TASK_STACK_SIZE       4096
#define DHT11_TASK_PRIORITY         5
#define DHT11_TASK_CORE_ID         1

// MQ135 Sensor task
#define MQ135_TASK_STACK_SIZE       4096
#define MQ135_TASK_PRIORITY         5
#define MQ135_TASK_CORE_ID         1


#endif // TASKS_COMMON_H    