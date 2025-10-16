idf_component_register(SRCS main.c rgb_led.c wifi_app.c
                       PRIV_REQUIRES driver 
                       REQUIRES esp_netif esp_wifi nvs_flash
                       INCLUDE_DIRS ".")
