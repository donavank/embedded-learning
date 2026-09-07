#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "hal/gpio_types.h"
#include "lwip/apps/sntp.h"

#include "http_server.h"
#include "tasks_common.h"

static const char TAG[] = "sntp_time_sync";
static bool sntp_op_mode_set = false;

static void sntp_time_sync_init_sntp(void) {
  ESP_LOGI(TAG, "Initializing SNTP time service...");
  if (!sntp_op_mode_set) {
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_op_mode_set = true;
  }

  sntp_setservername(0, "pool.ntp.org");
  sntp_init();
  http_server_monitor_send_message(HTTP_MSG_SNTP_INIT);
}

char *sntp_time_sync_get_time() {
  static char time_buffer[100];

  time_t now = 0;
  struct tm time_info = {0};
  time(&now);
  localtime_r(&now, &time_info);

  if (time_info.tm_year < 2016 - 1900) {
    sntp_time_sync_init_sntp();
    setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
    tzset();
  } else {
    strftime(time_buffer, sizeof(time_buffer), "%d.%m.%y %H:%M:%S", &time_info);
  }
  return time_buffer;
}

static void sntp_time_sync_obtain_time() {
  time_t now = 0;
  struct tm time_info = {0};
  time(&now);
  localtime_r(&now, &time_info);

  if (time_info.tm_year < 2026 - 1900) {
    sntp_time_sync_init_sntp();
    setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
    tzset();
  }
}

static void sntp_time_sync_task(void *pvParams) {
  for (;;) {
    sntp_time_sync_obtain_time();
    vTaskDelay(10000 * portTICK_PERIOD_MS);
  }

  vTaskDelete(NULL);
}

void sntp_time_sync_task_start() {
  xTaskCreatePinnedToCore(&sntp_time_sync_task, "sntp_time_sync_task",
                          SNTP_TIME_SYNC_TASK_STACK_SIZE, NULL,
                          SNTP_TIME_SYNC_TASK_PRIORITY, NULL,
                          SNTP_TIME_SYNC_TASK_CORE_ID);
}
