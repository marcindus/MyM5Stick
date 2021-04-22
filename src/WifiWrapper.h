#pragma once

#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"

#include "IWifiWrapper.h"

class WifiWrapper : public IWifiWrapper
{
    /*
    Simple wrapper for esp_wifi, for API documentation check:  doc/esp_wifi.h
    */

public:
    WifiWrapper()
    {
        //TO DO add debug log 
        //TO DO test for failure init 
        ESP_ERROR_CHECK(esp_wifi_set_country(&default_country));
        ESP_ERROR_CHECK(esp_wifi_init(&default_config));
        ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
        ESP_ERROR_CHECK(esp_wifi_start());
    };

    ~WifiWrapper()
    {
        ESP_ERROR_CHECK(esp_wifi_stop());
        ESP_ERROR_CHECK(esp_wifi_deinit());
    }

    void _esp_wifi_init(wifi_init_config_t) override;
    void _esp_wifi_set_country(wifi_country_t) override;
    void _esp_wifi_set_storage(wifi_storage_t) override;
    void _esp_wifi_set_mode(wifi_mode_t) override;
    void _esp_wifi_start() override;
    void _esp_wifi_stop() override;
    void _esp_wifi_set_promiscuous(bool) override;
    void _esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t) override;
    void _esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t secondary) override;

private:

    const wifi_country_t default_country = {.cc = "CN", .schan = 1, .nchan = 13};
    const wifi_init_config_t default_config  = WIFI_INIT_CONFIG_DEFAULT();

};


