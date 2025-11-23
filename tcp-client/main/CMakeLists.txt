idf_component_register(SRCS "tcp_client_main.c"
                                INCLUDE_DIRS "."
                                PRIV_REQUIRES esp_netif nvs_flash wifi_connect)
