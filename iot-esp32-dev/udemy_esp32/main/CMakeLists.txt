idf_component_register(SRCS "udemy_esp32.c" "rgb_led.c" "wifi_app.c" "http_server.c" "DHT11.c"
                    INCLUDE_DIRS "."
                    EMBED_FILES "webpage/app.css" "webpage/app.js" "webpage/index.html" "webpage/favicon.ico" "webpage/jquery-3.3.1.min.js")
