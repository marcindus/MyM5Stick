#pragma once

/*
https://docs.m5stack.com/en/core/m5stickc
https://carvesystems.com/news/writing-a-simple-esp8266-based-sniffer/
https://blog.podkalicki.com/wp-content/uploads/2017/01/esp32_promiscuous_pkt_structure.jpeg
https://github.com/SHA2017-badge/bpp/blob/master/esp32-recv/main/bpp_sniffer.c
https://github.com/n0w/esp8266-simple-sniffer/blob/master/src/main.cpp

Details about beacon frames: 
    https://mrncciew.com/2014/10/08/802-11-mgmt-beacon-frame/
*/

#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#define WIFI_CHANNEL_SWITCH_INTERVAL (500)
#define WIFI_CHANNEL_MAX (13)

uint8_t level = 0, channel = 1;

static wifi_country_t wifi_country = {.cc = "CN", .schan = 1, .nchan = 13};

typedef struct
{
    unsigned protocol : 2;
    unsigned type : 2;
    unsigned subtype : 4;
    unsigned to_ds : 1;
    unsigned from_ds : 1;
    unsigned more_frag : 1;
    unsigned retry : 1;
    unsigned pwr_mgmt : 1;
    unsigned more_data : 1;
    unsigned wep : 1;
    unsigned strict : 1;
} wifi_header_frame_control_t;

typedef struct
{
    wifi_header_frame_control_t frame_ctrl;
    unsigned duration_id : 16;
    uint8_t addr1[6]; /* receiver MAC address */
    uint8_t addr2[6]; /* sender MAC address */
    uint8_t addr3[6]; /* BSSID filtering address */
    unsigned sequence_ctrl : 16;
    uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct
{
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

typedef struct
{
    unsigned interval : 16;
    unsigned capability : 16;
    unsigned tag_number : 8;
    unsigned tag_length : 8;
    char ssid[0];
    uint8_t rates[1];
} wifi_beacon_hdr;

typedef struct
{
    uint8_t mac[6];
} __attribute__((packed)) mac_addr;

typedef enum
{
    ASSOCIATION_REQ,
    ASSOCIATION_RES,
    REASSOCIATION_REQ,
    REASSOCIATION_RES,
    PROBE_REQ,
    PROBE_RES,
    NU1, /* ......................*/
    NU2, /* 0110, 0111 not used */
    BEACON,
    ATIM,
    DISASSOCIATION,
    AUTHENTICATION,
    DEAUTHENTICATION,
    ACTION,
    ACTION_NACK,
} wifi_mgmt_subtypes_t;

static esp_err_t event_handler(void* ctx, system_event_t* event);
static void wifi_sniffer_init(void);
static void wifi_sniffer_start(void);
static void wifi_sniffer_stop(void);
static void wifi_sniffer_set_channel(uint8_t channel);
static const char* wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
static void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type);

esp_err_t event_handler(void* ctx, system_event_t* event)
{
    return ESP_OK;
}

Beacons beacons{5};

void wifi_sniffer_init(void)
{
    nvs_flash_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_country(&wifi_country));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void wifi_sniffer_start(void)
{
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void wifi_sniffer_stop(void)
{
    esp_wifi_set_promiscuous(true);
}

void wifi_sniffer_set_channel(uint8_t channel)
{
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

const char* wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
    switch (type)
    {
        case WIFI_PKT_MGMT:
            return "MGMT";
        case WIFI_PKT_DATA:
            return "DATA";
        default:
        case WIFI_PKT_MISC:
            return "MISC";
    }
}

void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
    if (type != WIFI_PKT_MGMT)
        return;

    const wifi_promiscuous_pkt_t* ppkt = (wifi_promiscuous_pkt_t*)buff;
    const wifi_ieee80211_packet_t* ipkt = (wifi_ieee80211_packet_t*)ppkt->payload;
    const wifi_ieee80211_mac_hdr_t* hdr = &ipkt->hdr;

    char ssid[32] = {0};

    const wifi_header_frame_control_t* fctl = 
            (wifi_header_frame_control_t*)&hdr->frame_ctrl;

    if (fctl->subtype == BEACON)
    { 
        wifi_beacon_hdr* beacon = (wifi_beacon_hdr*)ipkt->payload;
        if (beacon->tag_length >= 32)
        {
            strncpy(ssid, beacon->ssid, 31);
        }
        else
        {
            strncpy(ssid, beacon->ssid, beacon->tag_length);
        }
        beacons.add(ppkt->rx_ctrl.rssi);

/*
        Serial.printf("BEACON %s, ", ssid);
        Serial.printf("PACKET TYPE=%s, CHAN=%02d, RSSI=%02d,"
                      " SENDER=%02x:%02x:%02x:%02x:%02x:%02x,"
                      " RECIVER=%02x:%02x:%02x:%02x:%02x:%02x,"
                      " FILTER=%02x:%02x:%02x:%02x:%02x:%02x\n",
                      wifi_sniffer_packet_type2str(type),
                      ppkt->rx_ctrl.channel,
                      ppkt->rx_ctrl.rssi,

                      hdr->addr1[0],
                      hdr->addr1[1],
                      hdr->addr1[2],
                      hdr->addr1[3],
                      hdr->addr1[4],
                      hdr->addr1[5],

                      hdr->addr2[0],
                      hdr->addr2[1],
                      hdr->addr2[2],
                      hdr->addr2[3],
                      hdr->addr2[4],
                      hdr->addr2[5],

                      hdr->addr3[0],
                      hdr->addr3[1],
                      hdr->addr3[2],
                      hdr->addr3[3],
                      hdr->addr3[4],
                      hdr->addr3[5]);
                      */
    }
}


