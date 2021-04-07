#pragma once

bool checkOUTIO()  //Not used - looks like some voltage  check

{
    uint32_t sumadc1 = 0, sumadc2 = 0;
    pinMode(26,INPUT);

    for (int i = 0; i < 50; i++)
    {
        sumadc2 += analogRead(36);
        delay(50);
    }

    sumadc1 = sumadc1 / 50;
    sumadc2 = sumadc2 / 50;

    sumadc1 = sumadc1 * 3300 / 4095 * 2;
    sumadc2 = sumadc2 * 3300 / 4095 * 2;

    printf("%d,%d\n", sumadc1, sumadc2);

    if ( sumadc1 < 3000 )
    {
        ErrorMeg(0x61, "5V or 26 error");
    }
    if ( sumadc2 < 2500 )
    {
        ErrorMeg(0x62, "3V3 or 36 error");
    }
    return true;
}


bool CheckGrove()
{
    float tmp = dht12.readTemperature();
    float hum = dht12.readHumidity();
    uint8_t count = 0;

    for (int i = 0; i < 10; i++)
    {
        tmp = dht12.readTemperature();
        hum = dht12.readHumidity();
        if(( tmp > 50 )||( tmp < -20 )||( hum > 100 )||( hum == 0 ))
        {
            count ++;
            if( count > 5 )
            {
                ErrorMeg(0x91, "Grove error");
                return false;
            }
        }
    }
    return true;
}


