#pragma once

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <memory>

typedef struct
{
    unsigned frame_ctrl : 16;
    unsigned duration_id : 16;
    uint8_t addr1[6]; /* receiver address */
    uint8_t addr2[6]; /* sender address */
    uint8_t addr3[6]; /* filtering address */
    unsigned sequence_ctrl : 16;
    uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct
{
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

class ISniffer
{
public:
    virtual ~ISniffer(){};
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void on_packet() = 0;
    virtual void print_results() = 0;
    virtual uint8_t get_channel() = 0;
    virtual void set_channel(uint8_t) = 0;
};

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


class WifiSniffer : ISniffer
{
public:
    WifiSniffer(std::unique_ptr<IWifiWrapper> wifi_wrapper) : wifi_wrapper(std::move(wifi_wrapper))
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

private:
    static esp_err_t event_handler(void* ctx, system_event_t* event);
    static void wifi_sniffer_init(void);
    static void wifi_sniffer_set_channel(uint8_t channel);
    static const String sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
    static void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type);
    std::unique_ptr<IWifiWrapper> wifi_wrapper;
};

void WifiSniffer::start()
{
    esp_wifi_set_promiscuous(true);
    wifi_wrapper->_esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void WifiSniffer::stop()
{
    esp_wifi_set_promiscuous(false);
}

esp_err_t WifiSniffer::event_handler(void* ctx, system_event_t* event)
{
    return ESP_OK;
}

void WifiSniffer::wifi_sniffer_set_channel(uint8_t channel)
{
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

const String WifiSniffer::sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
    switch (type)
    {
        case WIFI_PKT_MGMT:
            return F("MGMT");
        case WIFI_PKT_DATA:
            return F("DATA");
        default:
        case WIFI_PKT_MISC:
            return F("MISC");
    }
}

void WifiSniffer::wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
    if (type != WIFI_PKT_MGMT)
        return;

    const wifi_promiscuous_pkt_t* ppkt = (wifi_promiscuous_pkt_t*)buff;
    const wifi_ieee80211_packet_t* ipkt = (wifi_ieee80211_packet_t*)ppkt->payload;
    const wifi_ieee80211_mac_hdr_t* hdr = &ipkt->hdr;

    printf("PACKET TYPE=%s, CHAN=%02d, RSSI=%02d,"
           " ADDR1=%02x:%02x:%02x:%02x:%02x:%02x,"
           " ADDR2=%02x:%02x:%02x:%02x:%02x:%02x,"
           " ADDR3=%02x:%02x:%02x:%02x:%02x:%02x\n",
           sniffer_packet_type2str(type),
           ppkt->rx_ctrl.channel,
           ppkt->rx_ctrl.rssi,
           /* ADDR1 */
           hdr->addr1[0],
           hdr->addr1[1],
           hdr->addr1[2],
           hdr->addr1[3],
           hdr->addr1[4],
           hdr->addr1[5],
           /* ADDR2 */
           hdr->addr2[0],
           hdr->addr2[1],
           hdr->addr2[2],
           hdr->addr2[3],
           hdr->addr2[4],
           hdr->addr2[5],
           /* ADDR3 */
           hdr->addr3[0],
           hdr->addr3[1],
           hdr->addr3[2],
           hdr->addr3[3],
           hdr->addr3[4],
           hdr->addr3[5]);
}

void SnifferLoop()
{

    Serial.begin(115200);
    delay(10);
    WifiWrapper wrapper{};
    WifiSniffer sniffer(&wrapper);
    delay(1000); // wait for a second
                 // wifi_sniffer_set_channel(channel);
    // channel = (channel % WIFI_CHANNEL_MAX) + 1; //scanning channels in a loop - businnes logic
}
