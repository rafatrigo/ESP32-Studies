idf_component_register(SRCS "wifi_connection.c"
                       PRIV_REQUIRES esp_wifi esp_event nvs_flash
                       INCLUDE_DIRS ".")
