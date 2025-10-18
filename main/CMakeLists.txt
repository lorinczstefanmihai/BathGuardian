idf_component_register(
                        SRCS main.c rgb_led.c wifi_app.c http_server.c DHT11.c MQ135.c
                        REQUIRES esp_netif esp_wifi nvs_flash esp_http_server app_update esp_timer driver esp_adc
                        INCLUDE_DIRS "."
                        EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.3.1.min.js)
