#pragma once

class IWifiWrapper
{
public:
    virtual ~IWifiWrapper(){};
    virtual void _esp_wifi_init(wifi_init_config_t) = 0; //Alloc resource for WiFi driver, such as WiFi control structure, RX/TX buffer,
    virtual void _esp_wifi_set_country(wifi_country_t) = 0;
    virtual void _esp_wifi_set_storage(wifi_storage_t) = 0;
    virtual void _esp_wifi_set_mode(wifi_mode_t) = 0;
    virtual void _esp_wifi_start() = 0;
    virtual void _esp_wifi_stop() = 0;
    virtual void _esp_wifi_set_promiscuous(bool) = 0;
    virtual void _esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t) = 0;
    virtual void _esp_wifi_set_channel(uint8_t, wifi_second_chan_t) = 0;

};

