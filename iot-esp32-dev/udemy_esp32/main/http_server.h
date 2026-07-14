
/*
 * http_server.h
 *
 */

#ifndef MAIN_HTTP_SERVER_H_
#define MAIN_HTTP_SERVER_H_

#include "freertos/FreeRTOS.h"

#define OTA_UPDATE_PENDING 0
#define OTA_UPDATE_SUCCESSFUL 1
#define OTA_UPDATE_FAILED -1

/**
 * Messages for the HTTP monitor
 */
typedef enum http_server_message {
  HTTP_MSG_WIFI_CONNECT_INIT = 0,
  HTTP_MSG_WIFI_CONNECT_SUCCESS,
  HTTP_MSG_WIFI_CONNECT_FAILED,
  HTTP_MSG_WIFI_OTA_UPDATE_SUCCESSFUL,
  HTTP_MSG_WIFI_OTA_UPDATE_FAILED,
} http_server_message_e;

/**
 * Structure for the message
 */
typedef struct http_server_queue_message {
  http_server_message_e msgId;
} http_server_queue_message_t;

/**
 * Sends a message to the http
 * @param msgId message ID from the http_server_message_e
 * @return pdTrue if the message was successfully submitted
 *         pdFalse otherwise
 * @note expand parameters based on requirements
 */
BaseType_t http_server_monitor_send_message(http_server_message_e);

/**
 * starts the HTTP server
 */
void http_server_start(void);

/**
 * stops the HTTP server
 */
void http_server_stop(void);

/**
 * Timer callback function which calls esp restart after successful firmware
 * update
 */
void http_server_fw_update_reset_callback(void *);

#endif /* MAIN_RGB_LED_H_ */
