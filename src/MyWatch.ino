#include "Setup.hpp"

void setup()
{
    M5.begin();
    M5.update();
    M5.Lcd.setRotation(3);
    M5.Lcd.setSwapBytes(false);
    Disbuff.createSprite(160, 80);
    Disbuff.setSwapBytes(true);

    Disbuff.fillRect(0, 0, 160, 80, BLACK);
    Displaybuff();

}

void loop()
{
    delay(100);
    Sniffer();
    DisplayRTC();
}
