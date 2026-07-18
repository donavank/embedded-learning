/*------------------------------------------------------------------------------

        DHT22 temperature & humidity sensor AM2302 (DHT22) driver for ESP32

        Jun 2017:	Ricardo Timmermann, new for DHT22

        Code Based on Adafruit Industries and Sam Johnston and Coffe & Beer.
Please help to improve this code.

        This example code is in the Public Domain (or CC0 licensed, at your
option.)

        Unless required by applicable law or agreed to in writing, this
        software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
        CONDITIONS OF ANY KIND, either express or implied.

        PLEASE KEEP THIS CODE IN LESS THAN 0XFF LINES. EACH LINE MAY CONTAIN ONE
BUG !!!

---------------------------------------------------------------------------------*/

/**
 * I have modified this code with the help of claude to work with the DHT11
 * sensor that my learning kit came with
 */

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include <stdio.h>

#include "DHT11.h"
#include "tasks_common.h"

// == global defines =============================================

static const char *TAG = "DHT";

int DHTgpio = 4; // my default DHT pin = 4
float humidity = 0.;
float temperature = 0.;

// == set the DHT used pin=========================================

void setDHTgpio(int gpio) { DHTgpio = gpio; }

// == get temp & hum =============================================

float getHumidity() { return humidity; }
float getTemperature() { return temperature; }

// == error handler ===============================================

void errorHandler(int response) {
  switch (response) {

  case DHT_TIMEOUT_ERROR:
    ESP_LOGE(TAG, "Sensor Timeout\n");
    break;

  case DHT_CHECKSUM_ERROR:
    ESP_LOGE(TAG, "CheckSum error\n");
    break;

  case DHT_OK:
    break;

  default:
    ESP_LOGE(TAG, "Unknown error\n");
  }
}

/*-------------------------------------------------------------------------------
;
;	get next state
;
;	I don't like this logic. It needs some interrupt blocking / priority
;	to ensure it runs in realtime.
;
;--------------------------------------------------------------------------------*/

int getSignalLevel(int usTimeOut, bool state) {

  int uSec = 0;
  while (gpio_get_level(DHTgpio) == state) {

    if (uSec > usTimeOut)
      return -1;

    ++uSec;
    ets_delay_us(1); // uSec delay
  }

  return uSec;
}

/*----------------------------------------------------------------------------
;
;	read DHT22 sensor

copy/paste from AM2302/DHT22 Docu:

DATA: Hum = 16 bits, Temp = 16 Bits, check-sum = 8 Bits

Example: MCU has received 40 bits data from AM2302 as
0000 0010 1000 1100 0000 0001 0101 1111 1110 1110
16 bits RH data + 16 bits T data + check sum

1) we convert 16 bits RH data from binary system to decimal system, 0000 0010
1000 1100 → 652 Binary system Decimal system: RH=652/10=65.2%RH

2) we convert 16 bits T data from binary system to decimal system, 0000 0001
0101 1111 → 351 Binary system Decimal system: T=351/10=35.1°C

When highest bit of temperature is 1, it means the temperature is below 0 degree
Celsius. Example: 1000 0000 0110 0101, T= minus 10.1°C: 16 bits T data

3) Check Sum=0000 0010+1000 1100+0000 0001+0101 1111=1110 1110 Check-sum=the
last 8 bits of Sum=11101110

Signal & Timings:

The interval of whole process must be beyond 2 seconds.

To request data from DHT:

1) Sent low pulse for > 1~10 ms (MILI SEC)
2) Sent high pulse for > 20~40 us (Micros).
3) When DHT detects the start signal, it will pull low the bus 80us as response
signal, then the DHT pulls up 80us for preparation to send data. 4) When DHT is
sending data to MCU, every bit's transmission begin with low-voltage-level that
last 50us, the following high-voltage-level signal's length decide the bit is
"1" or "0". 0: 26~28 us 1: 70 us

;----------------------------------------------------------------------------*/

#define MAXdhtData 5 // to complete 40 = 5*8 Bits

int readDHT() {
  int uSec = 0;
  int responseLow = 0, responseHigh = 0;

  uint8_t dhtData[MAXdhtData];
  uint8_t byteInx = 0;
  uint8_t bitInx = 7;

  for (int k = 0; k < MAXdhtData; k++)
    dhtData[k] = 0;

  // == Send start signal to DHT sensor ===========

  // pull down for 18ms (DHT11 minimum), then release HIGH — no direction switch
  // needed because the pin is configured as open-drain at init
  // 18ms LOW is long enough that preemption here doesn't matter
  gpio_set_direction(DHTgpio, GPIO_MODE_OUTPUT);
  gpio_set_level(DHTgpio, 0);
  ets_delay_us(18000);
  gpio_set_level(DHTgpio, 1);
  ets_delay_us(25);
  gpio_set_direction(DHTgpio, GPIO_MODE_INPUT);

  // == Wait for DHT to begin its response (line drops LOW after MCU releases)
  // ====

  uSec = getSignalLevel(90, 1);
  if (uSec < 0)
    return DHT_TIMEOUT_ERROR;

  // == DHT pulls LOW for ~80us then HIGH for ~80us ====

  responseLow = getSignalLevel(90, 0);
  if (responseLow < 0)
    return DHT_TIMEOUT_ERROR;

  // -- 80us up ------------------------

  responseHigh = getSignalLevel(90, 1);
  if (responseHigh < 0)
    return DHT_TIMEOUT_ERROR;

  // == No errors, read the 40 data bits ================

  for (int k = 0; k < 40; k++) {

    // -- starts new data transmission with >50us low signal

    uSec = getSignalLevel(100, 0);
    if (uSec < 0)
      return DHT_TIMEOUT_ERROR;

    // -- check to see if after >70us rx data is a 0 or a 1

    uSec = getSignalLevel(100, 1);
    if (uSec < 0)
      return DHT_TIMEOUT_ERROR;

    // add the current read to the output data
    // since all dhtData array where set to 0 at the start,
    // only look for "1" (>28us us)

    if (uSec > 40) {
      dhtData[byteInx] |= (1 << bitInx);
    }

    // index to next byte

    if (bitInx == 0) {
      bitInx = 7;
      ++byteInx;
    } else
      bitInx--;
  }

  ESP_LOGI(TAG, "Response = %d", responseLow);
  ESP_LOGI(TAG, "Response = %d", responseHigh);

  // == get humidity from Data[0] and Data[1] ==========================
  // DHT11: byte 0 = integer part, byte 1 = decimal part (tenths)
  // DHT22 combined them into a single 16-bit value divided by 10 — DHT11 does
  // not

  humidity = dhtData[0] + dhtData[1] / 10.0f;

  // == get temp from Data[2] and Data[3]

  temperature = (dhtData[2] & 0x7F) + dhtData[3] / 10.0f;

  if (dhtData[2] & 0x80) // negative temp, brrr it's freezing
    temperature *= -1;

  // == verify if checksum is ok ===========================================
  // Checksum is the sum of Data 8 bits masked out 0xFF

  if (dhtData[4] ==
      ((dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3]) & 0xFF))
    return DHT_OK;

  else
    return DHT_CHECKSUM_ERROR;
}

/*
 * START MY FUNCTIONS
 */
static TaskHandle_t task_dht_monitor = NULL;

static void dht_task(void *pvParameters) {
  printf("Starting DHT Task.");
  setDHTgpio(DHT_GPIO);
  // Do some stuff here maybe?
  for (;;) {
    printf("Reading DHT sensor...\n");
    int ret = readDHT();

    errorHandler(ret);

    printf("Humidity: %.1f\n", getHumidity());
    printf("Temp: %.1f\n", getTemperature());
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void dht_task_start(void) {

  xTaskCreatePinnedToCore(
      &dht_task, "dht_monitor_task", DHT_MONITOR_TASK_STACK_SIZE, NULL,
      DHT_MONITOR_TASK_PRIORITY, &task_dht_monitor, DHT_MONITOR_TASK_CORE_ID);
}
