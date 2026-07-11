/*
 * wifi_app.h
 *
 */

#ifndef MAIN_WIFI_APP_H_
#define MAIN_WIFI_APP_H_

#include "esp_netif.h"
#include "freertos/FreeRTOS.h"

// WIFI application settings
#define WIFI_AP_SSID "ESP32_AP"         // AP Name
#define WIFI_AP_PASSWORD "password"     // AP Password
#define WIFI_AP_CHANNEL 1               // AP Channel
#define WIFI_AP_SSID_HIDDEN 0           // AP Visibility (0 is visible)
#define WIFI_AP_MAX_CONNECTIONS 5       // AP Max Clients
#define WIFI_AP_BEACON_INTERVAL 100     // AP Beacon broadcast interval
#define WIFI_AP_IP "192.168.0.1"        // AP Default IP
#define WIFI_AP_GATEWAY "192.168.0.1"   // AP Default Gateway
#define WIFI_AP_NETMASK "255.255.255.0" // AP Netmask
#define WIFI_AP_BANDWIDTH WIFI_BW20
#define WIFI_STA_POWER_SAVE WIFI_PS_NONE
#define MAX_SSID_LENGTH 32     // IEEE Standard
#define MAX_PASSWORD_LENGTH 64 // IEEE Standard
#define MAX_CONNECTION_RETRIES 5

extern esp_netif_t *esp_netif_sta;
extern esp_netif_t *esp_netif_ap;

/**
 * Message IDs for WIFI Application Task
 * @note expand based on application requirements
 */
typedef enum wifi_app_message {
  WIFI_APP_MSG_START_HTTP_SERVER = 0,
  WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
  WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
} wifi_app_message_e;

/**
 * Struct for message queue
 * @note expand based on app requirements
 */
typedef struct wifi_app_queue_message {
  wifi_app_message_e msgId;
} wifi_app_queue_message_t;

/**
 * Sends a message to the queue
 * @param msdId message ID from the wifi_app_message_e
 * @return pdTrue if an item was successfully sent to the queue
 *         otherwise pdFalse
 * @note Expand the parameter list based on app requirements
 */
BaseType_t wifi_app_send_message(wifi_app_message_e msgId);

/**
 * Starts the WiFi RTOS task
 */
void wifi_app_start(void);

#endif /* MAIN_WIFI_APP_H_ */
