#pragma once

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

static wifi_country_t wifi_country = {.cc = "CN", .schan = 1, .nchan = 13}; // Most recent esp32 library struct - country and channels range

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


class ISniffingStrategy
{

};

class IWifiWrapper
{
public:
    virtual ~IWifiWrapper(){};
    virtual void start() = 0;

    virtual esp_err_t _esp_wifi_init(wifi_init_config_t) = 0;
    virtual esp_err_t _esp_wifi_set_country(wifi_country_t) = 0;
    virtual esp_err_t _esp_wifi_set_storage(wifi_storage_t) = 0;
    virtual esp_err_t _esp_wifi_set_mode(wifi_mode_t) = 0;
    virtual esp_err_t _esp_wifi_start() = 0;
    virtual esp_err_t _esp_wifi_set_promiscuous(bool) = 0;
    virtual esp_err_t _esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t) = 0;
};

class WifiWrapper : IWifiWrapper
{
public:
    WifiWrapper(){};

    esp_err_t _esp_wifi_init(wifi_init_config_t) override;
    esp_err_t _esp_wifi_set_country(wifi_country_t) override;
    esp_err_t _esp_wifi_set_storage(wifi_storage_t) override;
    esp_err_t _esp_wifi_set_mode(wifi_mode_t) override;
    esp_err_t _esp_wifi_start() override;
    esp_err_t _esp_wifi_set_promiscuous(bool) override;
    esp_err_t _esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t) override;

private:
    //country config
    //

};

esp_err_t WifiWrapper::_esp_wifi_init(wifi_init_config_t cfg) 
{
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
};
esp_err_t WifiWrapper::_esp_wifi_set_country(wifi_country_t country) 
{
    //
};
esp_err_t WifiWrapper::_esp_wifi_set_storage(wifi_storage_t memory_storage) 
{
    //
};
esp_err_t WifiWrapper::_esp_wifi_set_mode(wifi_mode_t mode) 
{
    //
};
esp_err_t WifiWrapper::_esp_wifi_start() 
{
    //
};
esp_err_t WifiWrapper::_esp_wifi_set_promiscuous(bool set_promiscous) 
{
    //
};
esp_err_t WifiWrapper::_esp_wifi_set_promiscuous_rx_cb(wifi_promiscuous_cb_t cb) 
{
    ///
};


read this !!!!   esp32/esp_wifi.h 


class WifiSniffer : ISniffer
{
public:
    WifiSniffer();
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
};

WifiSniffer::WifiSniffer()
{
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_country(&wifi_country)); /* set country for channel range [1, 13] */
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void WifiSniffer::start()
{
    esp_wifi_set_promiscuous(true);
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
    // wifi_sniffer_init();
    WifiSniffer sniffer{};
    delay(1000); // wait for a second
                 // wifi_sniffer_set_channel(channel);
    // channel = (channel % WIFI_CHANNEL_MAX) + 1; //scanning channels in a loop - businnes logic
}