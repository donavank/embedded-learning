/*
 * rgb_led.c
 *   author: Donavan Keen
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_wifi_types_generic.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"
#include "wifi_app.h"

static const char TAG[] = "nvs";
const char app_nvs_sta_creds_namespace[] = "sta_creds";

esp_err_t app_nvs_save_sta_creds(void) {
  nvs_handle_t handle;
  esp_err_t esp_err;
  ESP_LOGI(TAG, "app_nvs_save_sta_creds: Saving station credentials to flash");

  wifi_config_t *config = wifi_app_get_wifi_config();
  if (config != NULL) {
    esp_err = nvs_open(app_nvs_sta_creds_namespace, NVS_READWRITE, &handle);
    if (esp_err != ESP_OK) {
      printf("app_nvs: Failed to open flash storage: %s",
             esp_err_to_name(esp_err));
      return esp_err;
    }

    esp_err = nvs_set_blob(handle, "ssid", config->sta.ssid, MAX_SSID_LENGTH);
    if (esp_err != ESP_OK) {
      printf("app_nvs: Failed to write SSID to flash storage: %s",
             esp_err_to_name(esp_err));
      nvs_close(handle);
      return esp_err;
    }

    esp_err = nvs_set_blob(handle, "password", config->sta.password,
                           MAX_PASSWORD_LENGTH);
    if (esp_err != ESP_OK) {
      printf("app_nvs: Failed to write password to flash storage: %s",
             esp_err_to_name(esp_err));
      nvs_close(handle);
      return esp_err;
    }

    esp_err = nvs_commit(handle);
    if (esp_err != ESP_OK) {
      printf("app_nvs: Failed to commit to flash storage: %s",
             esp_err_to_name(esp_err));
      nvs_close(handle);
      return esp_err;
    }

    nvs_close(handle);
  }
  return ESP_OK;
}

bool app_nvs_load_sta_creds(void) {
  nvs_handle_t handle;
  esp_err_t esp_err;
  ESP_LOGI(TAG,
           "app_nvs_load_sta_creds: Loading station credentials from flash");
  esp_err = nvs_open(app_nvs_sta_creds_namespace, NVS_READONLY, &handle);
  if (esp_err == ESP_OK) {
    wifi_config_t *config = wifi_app_get_wifi_config();

    if (config == NULL) {
      nvs_close(handle);
      return false;
    }

    memset(config, 0x00, sizeof(wifi_config_t));

    size_t read_size = sizeof(config->sta.ssid);

    esp_err = nvs_get_blob(handle, "ssid", config->sta.ssid, &read_size);
    if (esp_err != ESP_OK) {
      printf("app_nvs: Failed to read ssid from flash storage: %s",
             esp_err_to_name(esp_err));
      nvs_close(handle);
      return false;
    }

    read_size = sizeof(config->sta.password);
    esp_err =
        nvs_get_blob(handle, "password", config->sta.password, &read_size);
    if (esp_err != ESP_OK) {
      printf("app_nvs: Failed to read password from flash storage: %s",
             esp_err_to_name(esp_err));
      nvs_close(handle);
      return false;
    }

    nvs_close(handle);
  } else {
    printf("app_nvs: Failed to open flash storage for read: %s",
           esp_err_to_name(esp_err));
    return false;
  }
  return true;
}

esp_err_t app_nvs_clear_sta_creds(void) {
  nvs_handle_t handle;
  esp_err_t esp_err;
  ESP_LOGI(TAG,
           "app_nvs_clear_sta_creds: Erasing station credentials from flash");
  esp_err = nvs_open(app_nvs_sta_creds_namespace, NVS_READWRITE, &handle);

  if (esp_err != ESP_OK) {
    printf("app_nvs: Failed to open flash storage for erasure: %s",
           esp_err_to_name(esp_err));
    return esp_err;
  }

  esp_err = nvs_erase_all(handle);
  if (esp_err != ESP_OK) {
    printf("app_nvs: Failed to erase flash storage: %s",
           esp_err_to_name(esp_err));
    nvs_close(handle);
    return esp_err;
  }

  esp_err = nvs_commit(handle);
  if (esp_err != ESP_OK) {
    printf("app_nvs: Failed to commit flash storage: %s",
           esp_err_to_name(esp_err));
    nvs_close(handle);
    return esp_err;
  }

  nvs_close(handle);
  return ESP_OK;
}
