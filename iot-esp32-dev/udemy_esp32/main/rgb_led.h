/*
 * rgb_led.h
 *
 */

#ifndef MAIN_RGB_LED_H_
#define MAIN_RGB_LED_H_

// RGB LED GPIO
#define RGB_LED_RED_GPIO 2
#define RGB_LED_GREEN_GPIO 4
#define RGB_LED_BLUE_GPIO 5

// RGB LED Color Mix Channels
#define RGB_LED_CHANNEL_NUM 3

// RGB LED Configuration
typedef struct {
  int channel;
  int gpio;
  int mode;
  int timer_index;
} ledc_info_t;

/*
 * Color to indicate wifi has started
 */
void rgb_led_wifi_app_started(void);

/*
 * Color to indicate http server has started
 */
void rgb_led_http_server_started(void);

/*
 * Color to indicate wifi has connected to an access point
 */
void rgb_led_wifi_connected(void);

#endif /* MAIN_RGB_LED_H_ */
