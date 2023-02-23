#pragma once 
#include <M5StickC.h>

int n;
int ssidLength = 10;
int thisPage = 0;
const int pageSize = 8;
bool on = false;

void LCD_Clear();
void Show(int);
void PleaseWaitSearching();
void InitLCD();

void InitLCD()
{
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(45, 20);
    M5.Lcd.printf("Wi-Fi");
    M5.Lcd.setCursor(30, 45);
    M5.Lcd.printf("scanner");
}

void PleaseWaitSearching()
{
    on = true;
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
}

void Show(int nav = 0)
{
    if ((nav == 1) && (on == true))
    {
        if ((thisPage) < ((n - 1) / pageSize))
        {
            thisPage++;
        }
        else
        {
            thisPage = 0;
        }
        Show();
    }
    else
    {
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

void LCD_Clear()
{
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
}


