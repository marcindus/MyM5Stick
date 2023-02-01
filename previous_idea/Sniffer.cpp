#include "Sniffer.hpp"

void Sniffer::scan_loop()
{
    // channel = (channel % WIFI_CHANNEL_MAX) + 1; //scanning channels in a loop - businnes logic
    // sleep ?
    // set channel
}

void Sniffer::start()
{
    wifi_wrapper->set_promiscuous(true);
    wifi_wrapper->set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void Sniffer::stop()
{
    wifi_wrapper->set_promiscuous(false);
}

esp_err_t Sniffer::event_handler(void* ctx, system_event_t* event)
{
    return ESP_OK;
}

void Sniffer::wifi_sniffer_set_channel(uint8_t channel)
{
    wifi_wrapper->set_channel(channel);
}

const String Sniffer::sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
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

void Sniffer::wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
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

