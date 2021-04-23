
#include "WifiWrapper.h"

void WifiWrapper::set_promiscuous(bool set_promiscous) 
{
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(set_promiscous));
};

void WifiWrapper::set_promiscuous_rx_cb(wifi_promiscuous_cb_t cb) 
{
    ESP_ERROR_CHECK(esp_wifi_set_promiscuous_rx_cb(cb));
};

void WifiWrapper::set_channel(uint8_t primary) 
{
    ESP_ERROR_CHECK(esp_wifi_set_channel(primary, WIFI_SECOND_CHAN_NONE));
};


