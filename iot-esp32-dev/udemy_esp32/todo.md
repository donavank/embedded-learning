# To Do

List of things to do by Lesson

## DHT Sensor Lesson 1

- Import DHT-22 header and source files.
  - Add them to the cmakelists
    - I've concluded that these should work interchangeably, the only difference seems to be the frequency that you can read from the sensors -- DHT-11 can be read at 1hz while DHT-22 is 0.5hz or less

## WiFi Connect Implementation Part 1

- Add WiFi connect section to the webpage
  - Section with inputs for:
    - connect_ssid maxlength 32, type text, maxlength 32, placeholder SSID, value ""
    - connect_ssid maxlength 64, type password, placeholder password, value ""
    - checkbox (to show password)
  - Button to connect
  - Section for errors
- Update styles
  - Add gr for green
  - Add rd for red
- Update JS
  - Connect onClick function for the connect WiFi
  - Variable to track wifiConnectInterval
  - stopWifiConnectStatusInterval() - stops polling the wifi status (when disconnected)
  - getWifiConnectStatus() - gets wifi connection status from the esp
    - uses xml http request for this one.
  - startWifiConnectStatusInterval() - polls the wifi connection status
  - connectWifi() - calls the http server function to connect the esp 32 to wifi
  - checkCredentials - checks the wifi credentials on connect_wifi button click, cannot be blank, sets errors to list if not empty, calls connect wifi

## WiFi Connect Implementation Part 2

- Create URI handlers for new routes
  - /wifiConnect.json - HTTP_POST, http_server_wifi_connect_json_handler - receives
    the ssid and password, return ESP_OK and begins the connection process in wifi app
    - Uses helper httpd_req_get_hdr_value_len to get header lengths before allocating
      buffers and reading them
    - Gets the wifi_config_t - sets to zero then memcpy to wifi_config->sta.ssid, wifi_config->sta.password
    - wifi_app_send_message(WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER)
    - free the buffers
  - /wifiConnectStatus - HTTP_POST, http_server_wifi_connect_status_json_handler -
- Utilize wificonnect status messages
- WiFi App Updates
  - Create wifi configuration (wifi_config_t) in the wifi_app.c file
    - Allocate in the wifi_app_start method
    - memset to zero
  - Create function prototype for wifi_config_t* wifi_app_get_wifi_config(void) - just returns the wifi config
  - Create global retry counter
  - http_server
    - http_server_wifi_connect_status enum - connecting, failed, success
    - global variable for tracking the status, updates in the monitor

Double-check that I mostly did things right

## WiFi Connect Implementation Part 3

- Add Logic under WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER case
  - call wifi_app_connect_sta() - function to be defined later
  - set g_retry_number to 0
  - send server monitor message HTTP_MSG_WIFI_CONNECT_INIT
- wifi_app_connect_sta
  - static void wifi_app_connect_sta(void)
  - error check the mothod esp_wifi_set_config
  - error check esp_wifi_connect
- Update event handler for the STA_DISCONNECTED case
  - get the wifi_event_sta_disconnected_t with some weird reference/casting stuff using the event_data
    - This is copying the contents of the event_data pointer into the pointer we created
      - `*disconnected_event = (wifi_event_sta_disconnected_t *)event_data;`
  - printf the reason wifi_event_sta_disconnected->reason
  - retry if below MAX_CONNECTION_RETRIES
  - else send wifi app the WIFI_APP_MSG_STA_DISCONNECTED
- Update IP_EVENT_STA_GOT_IP case to send message to wifi app
- Update the got ip case in the wifi app event handler
  - HTTP_SERVER_MSG_WIFI_CONNECT_SUCCESS - send this message to http server
- Update STA_DISCONNECTED case to send http_server fail message
- Create connectStatusJson handler to update connection status on the web page

## WiFi Connect Info

- FIX CONNECTION ISSUE. WHEN ESP32 tries to connect, not getting IP and my computer is disconnected -- DONE

- div for wifi info section with inner divs paired to each other
  - section - divs for: ip_address_label : wifi_connect_ip
  - netmask_label : wifi_connect_netmask
  - gateway_label : wifi_coonnct_gw
- buttons section with disconnect button
- "<hr/>"
- css classes for those sections
  - `#connected_ap_label, #connected_ap`
  - `#ip_address_label, #netmask_label, #gateway_label`
  - `#wifi_connect_ip, #wifi_connect_nextmask, #wifi_connect_gw`
  - all `display:inline;`
  - Set some colors and set display:none on the disconnect button
- getConnectInfo function method to wifiConnectionInfo.json
  - Get's the connection info for displaying on the webpage
  - Updates html elements to display that information
