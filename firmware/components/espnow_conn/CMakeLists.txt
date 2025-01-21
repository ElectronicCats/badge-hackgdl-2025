idf_component_register(SRCS "espnow_conn.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi
                    PRIV_REQUIRES nvs_flash )
