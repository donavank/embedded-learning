#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "hal/gpio_types.h"
#include "tasks_common.h"
#include "wifi_app.h"
#include "wifi_reset_button.h"

static const char TAG[] = "wifi_reset_button";

SemaphoreHandle_t wifi_reset_semaphore;

static void wifi_reset_button_isr_handler(void *arg) {
  xSemaphoreGiveFromISR(wifi_reset_semaphore, NULL);
}

static void wifi_reset_button_task(void *pvParameters) {

  for (;;) {
    if (xSemaphoreTake(wifi_reset_semaphore, portMAX_DELAY) == pdTRUE) {
      ESP_LOGI(TAG, "WIFI RESET BUTTON INTERRUPT OCCURRED");
      wifi_app_send_message(WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT);
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void wifi_reset_button_config(void) {
  wifi_reset_semaphore = xSemaphoreCreateBinary();

  esp_rom_gpio_pad_select_gpio(WIFI_RESET_BUTTON_GPIO);
  gpio_set_direction(WIFI_RESET_BUTTON_GPIO, GPIO_MODE_INPUT);
  gpio_set_intr_type(WIFI_RESET_BUTTON_GPIO, GPIO_INTR_NEGEDGE);
  gpio_install_isr_service(WIFI_RESET_BUTTON_INTR_FLAG_DEFAULT);

  xTaskCreatePinnedToCore(&wifi_reset_button_task, "wifi_reset_button_task",
                          WIFI_RESET_BUTTON_TASK_STACK_SIZE, NULL,
                          WIFI_RESET_BUTTON_TASK_PRIORITY, NULL,
                          WIFI_RESET_BUTTON_TASK_CORE_ID);

  gpio_isr_handler_add(WIFI_RESET_BUTTON_GPIO, wifi_reset_button_isr_handler,
                       NULL);
}
