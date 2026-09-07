idf_component_register(SRCS "udemy_esp32.c" "rgb_led.c" "wifi_app.c" "http_server.c" "DHT11.c" "app_nvs.c" "wifi_reset_button.c" "sntp_time_sync.c" "mqtt_demo_mutual_auth.c"
                    INCLUDE_DIRS "."
                    EMBED_FILES "webpage/app.css" "webpage/app.js" "webpage/index.html" "webpage/favicon.ico" "webpage/jquery-3.3.1.min.js"
                  )

                  target_add_binary_data(${COMPONENT_TARGET} "certs/aws_root_cert_ca_pem" TEXT)
                  target_add_binary_data(${COMPONENT_TARGET} "certs/certificate_pem_crt" TEXT)
                  target_add_binary_data(${COMPONENT_TARGET} "certs/private_pem_key" TEXT)
