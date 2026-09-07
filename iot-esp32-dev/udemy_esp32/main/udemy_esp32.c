#include "freertos/FreeRTOS.h"
#include <freertos/task.h>

#include "DHT11.h"
#include "demo_config.h"
#include "nvs_flash.h"
#include "sntp_time_sync.h"
#include "wifi_app.h"
#include "wifi_reset_button.h"

int aws_iot_demo_main(int argc, char **argv);

static void wifi_connected_callback() {
  sntp_time_sync_task_start();
  aws_iot_demo_main(0, NULL);
}

void app_main(void) {
  esp_err_t ret = nvs_flash_init();

  // Initialize NVS (non-volatile storage)
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
    ESP_ERROR_CHECK(ret);
  }

  wifi_app_start();
  wifi_reset_button_config();
  dht_task_start();
  wifi_app_set_connected_callback(&wifi_connected_callback);
}
