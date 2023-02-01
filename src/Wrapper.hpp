#pragma once

#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"

class Wrapper
{

public:
    Wrapper()
    {
        ESP_ERROR_CHECK(esp_wifi_set_country(&default_country));
        ESP_ERROR_CHECK(esp_wifi_init(&default_config));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
        ESP_ERROR_CHECK(esp_wifi_start());
    };

    ~Wrapper()
    {
        ESP_ERROR_CHECK(esp_wifi_stop());
        ESP_ERROR_CHECK(esp_wifi_deinit());
    }

    void set_promiscuous(bool);
    void set_promiscuous_rx_cb(wifi_promiscuous_cb_t);
    void set_channel(uint8_t primary);

private:
    const wifi_country_t default_country = {.cc = "PL", .schan = 1, .nchan = 13};
    const wifi_init_config_t default_config = WIFI_INIT_CONFIG_DEFAULT();
};
