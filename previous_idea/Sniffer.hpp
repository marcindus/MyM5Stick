#pragma once

#include "freertos/FreeRTOS.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <memory>
#include "Arduino.h"
#include "Wrapper.hpp"
#include "wifi_ieee80211_packet.h"

class Sniffer
{
public:
    Sniffer(std::unique_ptr<Wrapper> _wifi_wrapper) : wifi_wrapper(std::move(_wifi_wrapper))
    {
        tcpip_adapter_init();
        ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    }

    void start();
    void stop();
    void on_packet();
    void print_results();
    uint8_t get_channel();
    void set_channel(uint8_t);
    void scan_loop();

private:
    static esp_err_t event_handler(void* ctx, system_event_t* event);
    void wifi_sniffer_init(void);
    void wifi_sniffer_set_channel(uint8_t channel);
    static const String sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
    static void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type);
    std::unique_ptr<Wrapper> wifi_wrapper;
};
