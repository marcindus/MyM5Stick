//    note: Change Partition Scheme(Default -> NoOTA or MinimalSPIFFS)

#include <M5StickC.h>
#include "Arduino.h"
#include "WifiSniffer.h"

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

//  nvs_flash_init(); 

  //vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS);

void Displaybuff()
{
    Disbuff.pushSprite(0, 0);
}

void Sniffer()
{
    Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
    Displaybuff();
    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.drawString("sniffer start",32,5,1);
        Displaybuff();
        M5.update();
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}


void DisplayRTC()
{
    Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
    Displaybuff();
    M5.Rtc.GetBm8563Time();
    RTC_TimeTypeDef time;
    M5.Rtc.GetTime(&time);

    Disbuff.setTextSize(3);
    Disbuff.setCursor(6, 25);
    Disbuff.setTextColor(WHITE);

    while ((!M5.BtnA.isPressed()) && (!M5.BtnB.isPressed()))
    {
        Disbuff.fillRect(0, 0, 160, 80, Disbuff.color565(0, 0, 0));
        M5.Rtc.GetTime(&time);
        Disbuff.setTextSize(3);
        Disbuff.setTextColor(WHITE);
        Disbuff.setCursor(6, 40);
        Disbuff.printf("%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
        Disbuff.fillRect(0,0,160,18,Disbuff.color565(20,20,20));
        Disbuff.setTextSize(1);
        Disbuff.drawString("BMP8563 RTC Time",32,5,1);
        Displaybuff();
        M5.update();
        delay(100);
    }
    while ((M5.BtnA.isPressed()) || (M5.BtnB.isPressed()))
    {
        M5.update();
        delay(10);
    }
}



void setup()
{
    M5.begin();
    M5.update();
    Serial.println(F("Hello"));
    M5.Lcd.setRotation(3);
    M5.Lcd.setSwapBytes(false);
    Disbuff.createSprite(160, 80);
    Disbuff.setSwapBytes(true);

    Disbuff.fillRect(0,0,160,80,BLACK);
    Displaybuff();

}

void loop()
{
    delay(100);
    Sniffer();
    DisplayRTC();
}
