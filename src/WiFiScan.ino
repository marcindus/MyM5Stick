/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5StickC sample source code
*                          配套  M5StickC 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：Wifi scan.  wifi扫描
* date：2022/4/3
*******************************************************************************
  http://forum.m5stack.com/topic/58/lesson-3-wi-fi-scanner
  Press M5 to start/re-scan wifi, press up to turn down the page
  按下M5键以开始/重新扫描wifi,按下上键向下翻页
*/

#include <M5StickC.h>
//#include "WiFi.h"

#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"

#define WIFI_CHANNEL_SWITCH_INTERVAL  (500)
#define WIFI_CHANNEL_MAX               (13)

uint8_t level = 0, channel = 1;

static wifi_country_t wifi_country = {.cc="CN", .schan = 1, .nchan = 13}; //Most recent esp32 library struct

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

static esp_err_t event_handler(void *ctx, system_event_t *event);
static void wifi_sniffer_init(void);
static void wifi_sniffer_set_channel(uint8_t channel);
static const char *wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type);

esp_err_t event_handler(void *ctx, system_event_t *event)
{
  return ESP_OK;
}

void wifi_sniffer_init(void)
{
  nvs_flash_init();
  tcpip_adapter_init();
  ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_country(&wifi_country) ); /* set country for channel range [1, 13] */
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
  ESP_ERROR_CHECK( esp_wifi_start() );
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void wifi_sniffer_set_channel(uint8_t channel)
{
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

const char * wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
  switch(type) {
  case WIFI_PKT_MGMT: return "MGMT";
  case WIFI_PKT_DATA: return "DATA";
  default:  
  case WIFI_PKT_MISC: return "MISC";
  }
}

void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

  printf("PACKET TYPE=%s, CHAN=%02d, RSSI=%02d,"
    " ADDR1=%02x:%02x:%02x:%02x:%02x:%02x,"
    " ADDR2=%02x:%02x:%02x:%02x:%02x:%02x,"
    " ADDR3=%02x:%02x:%02x:%02x:%02x:%02x\n",
    wifi_sniffer_packet_type2str(type),
    ppkt->rx_ctrl.channel,
    ppkt->rx_ctrl.rssi,
    /* ADDR1 */
    hdr->addr1[0],hdr->addr1[1],hdr->addr1[2],
    hdr->addr1[3],hdr->addr1[4],hdr->addr1[5],
    /* ADDR2 */
    hdr->addr2[0],hdr->addr2[1],hdr->addr2[2],
    hdr->addr2[3],hdr->addr2[4],hdr->addr2[5],
    /* ADDR3 */
    hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
    hdr->addr3[3],hdr->addr3[4],hdr->addr3[5]
  );
}










int n;
int ssidLength     = 10;
int thisPage       = 0;
const int pageSize = 8;
bool on            = false;

void LCD_Clear() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
}

void Show(int nav = 0)  // -1 top, 1 bottom
{
    if ((nav == 1) && (on == true)) {
        if ((thisPage) < ((n - 1) / pageSize)) {
            thisPage++;
        } else {
            thisPage = 0;
        }
        Show();
    } else {
        LCD_Clear();
        M5.Lcd.setTextSize(1);
        M5.Lcd.setCursor(30, 5);
        M5.Lcd.print("TOTAL: ");
        M5.Lcd.print(n);
        M5.Lcd.setCursor(0, 16);
        /*
        for (int i = (thisPage * pageSize);
             i < ((thisPage * pageSize) + pageSize); i++) {
            if (i >= n) break;
            String ssid = (WiFi.SSID(i).length() > ssidLength)
                              ? (WiFi.SSID(i).substring(0, ssidLength) + "...")
                              : WiFi.SSID(i);
            M5.Lcd.print(" " + String(i + 1));
            M5.Lcd.print(") " + ssid + " (" + WiFi.RSSI(i) + ");\n");
        }
        */
    }
}

void Search() {
    on       = true;
    thisPage = 0;
    LCD_Clear();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(10, 20);
    M5.Lcd.setTextColor(TFT_YELLOW);
    M5.Lcd.printf("Please, wait");
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_ORANGE);
    M5.Lcd.setCursor(40, 50);
    M5.Lcd.printf("Searching...");
    //n = WiFi.scanNetworks();
    Show();
}

void setup() {
    M5.begin();
    //WiFi.mode(WIFI_STA);
    //WiFi.disconnect();
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(45, 20);
    M5.Lcd.printf("Wi-Fi");
    M5.Lcd.setCursor(30, 45);
    M5.Lcd.printf("scanner");


  Serial.begin(115200);
  delay(10);
  wifi_sniffer_init();

}

void loop() {
    if (M5.BtnA.wasPressed()) Search();
    if (M5.BtnB.wasPressed()) Show(1);



 //   delay(1000); // wait for a second
 //   vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS);
 //   wifi_sniffer_set_channel(channel);
 //   channel = (channel % WIFI_CHANNEL_MAX) + 1;



    M5.update();
}