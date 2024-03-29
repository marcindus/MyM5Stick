#include <M5StickC.h>
#include "freertos/FreeRTOS.h"
#include "Beacon.hpp"
#include "SnifferWifi.hpp"
#include "Lcd.hpp"



void setup()
{
    M5.begin();
    void InitLCD();
    Serial.begin(115200);
    delay(10);
    wifi_sniffer_init();
    wifi_sniffer_start();
}

void loop()
{
    /*
    if (M5.BtnA.wasPressed())
    {
        M5.Lcd.setCursor(45, 20);
        M5.Lcd.printf("scanning");
        wifi_sniffer_start();
    }
    if (M5.BtnB.wasPressed())
    {
        wifi_sniffer_stop();
    }
*/
    delay(1000);
    vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS); //Sleep 500
    Serial.printf("Channel %d\n", channel);
    beacons.print_me();

    beacons.clear();
    wifi_sniffer_set_channel(channel);
    channel = (channel % WIFI_CHANNEL_MAX) + 1;

    M5.update();
}
