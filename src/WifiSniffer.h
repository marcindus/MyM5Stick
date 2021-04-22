#pragma once

#include "freertos/FreeRTOS.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <memory>
#include "Arduino.h"
#include "WifiWrapper.h"
#include "ISniffer.h"
#include "wifi_ieee80211_packet.h"

class WifiSniffer : public ISniffer
{
public:
    WifiSniffer(std::unique_ptr<IWifiWrapper> _wifi_wrapper) : wifi_wrapper(std::move(_wifi_wrapper))
    {
        tcpip_adapter_init();   
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    }

    void start() override;
    void stop() override;
    void on_packet() override;
    void print_results() override;
    uint8_t get_channel() override;
    void set_channel(uint8_t) override;
    void scan_loop();

private:
    static esp_err_t event_handler(void* ctx, system_event_t* event);
    static void wifi_sniffer_init(void);
    static void wifi_sniffer_set_channel(uint8_t channel);
    static const String sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
    static void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type);
    std::unique_ptr<IWifiWrapper> wifi_wrapper;
};

