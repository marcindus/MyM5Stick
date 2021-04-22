
#include "WifiWrapper.h"

void WifiWrapper::_esp_wifi_init(wifi_init_config_t cfg) 
{
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
};

void WifiWrapper::_esp_wifi_set_country(wifi_country_t country) 
{
    ESP_ERROR_CHECK(esp_wifi_set_country(&country));
};

void WifiWrapper::_esp_wifi_set_storage(wifi_storage_t memory_storage) 
{
    ESP_ERROR_CHECK(esp_wifi_set_storage(memory_storage));
};

void WifiWrapper::_esp_wifi_set_mode(wifi_mode_t mode) 
{
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
};

void WifiWrapper::_esp_wifi_start() 
{
    ESP_ERROR_CHECK(esp_wifi_start());
};

void WifiWrapper::_esp_wifi_stop() 
{
    ESP_ERROR_CHECK(esp_wifi_stop());
};

void WifiWrapper::_esp_wifi_set_promiscuous(bool set_promiscous) 
{
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(set_promiscous));
};

void WifiWrapper::_esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t cb) 
{
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(cb));
};

void WifiWrapper::_esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t secondary) 
{
    ESP_ERROR_CHECK(esp_wifi_set_channel(primary,secondary));
};


