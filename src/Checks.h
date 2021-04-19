#pragma once

#include "Drawings.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "1bc68b2a-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_RX_UUID "1bc68da0-f3e3-11e9-81b4-2a2ae2dbcce4"
#define CHARACTERISTIC_TX_UUID "1bc68efe-f3e3-11e9-81b4-2a2ae2dbcce4"

BLEServer* pServer = NULL;
BLEService* pService = NULL;
BLECharacteristic* pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer) { deviceConnected = true; };

    void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};

uint8_t* data = new uint8_t[128];

class MyCallbacks : public BLECharacteristicCallbacks
{

    void onWrite(BLECharacteristic* pCharacteristic) { data = pCharacteristic->getData(); }
};

bool checkAXP192()
{
    float temp = M5.Axp.GetTempInAXP192();
    if ((temp < 5) || (temp > 90))
    {
        ErrorMeg(0x21, "AXP find error");
        return false;
    }

    float VBat = M5.Axp.GetBatVoltage();
    if ((VBat < 3.0) || (VBat > 4.4))
    {
        ErrorMeg(0x22, "Bat Vol error ");
        return false;
    }

    return true;
}

bool checkMPU6886()
{

    return true;
}

bool checkBM8563()
{
    uint8_t i = 0;
    M5.Rtc.GetBm8563Time();
    RTC_TimeTypeDef time;
    M5.Rtc.GetTime(&time);
    uint8_t sec = time.Seconds;
    while (1)
    {
        M5.Rtc.GetTime(&time);
        delay(10);
        i++;
        if (i > 200)
        {
            ErrorMeg(0x41, "RTC error ");
        }
        if (sec != time.Seconds)
        {
            break;
        }
    }
    return true;
}

#define PIN_CLK 0
#define PIN_DATA 34

bool InitI2SMicroPhone()
{
    esp_err_t err = ESP_OK;
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t pin_config;
    pin_config.bck_io_num = I2S_PIN_NO_CHANGE;
    pin_config.ws_io_num = PIN_CLK;
    pin_config.data_out_num = I2S_PIN_NO_CHANGE;
    pin_config.data_in_num = PIN_DATA;

    err += i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    err += i2s_set_pin(I2S_NUM_0, &pin_config);
    err += i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    // i2s_set_clk(0)

    if (err != ESP_OK)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool InitBLEServer()
{
    BLEDevice::init("M5-BLE");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    pService = pServer->createService(SERVICE_UUID);
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_RX_UUID, BLECharacteristic::PROPERTY_NOTIFY);

    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic* pRxCharacteristic =
        pService->createCharacteristic(CHARACTERISTIC_TX_UUID, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks());

    return true;
}

bool InitIRTx()
{
    /*
    esp_err_t err = esp_pm_lock_create(ESP_PM_APB_FREQ_MAX, 0, "rmt", &rmt_freq_lock);
    if (err != ESP_OK)
    {
        ErrorMeg(0x71, "RMT create fail");
        return false;
    }
    */
    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel = RMT_TX_CHANNEL;
    rmt_tx.gpio_num = RMT_TX_GPIO_NUM;

    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = RMT_CLK_DIV;

    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_duty_percent = 50;
    rmt_tx.tx_config.carrier_freq_hz = 38000;
    rmt_tx.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
    rmt_tx.tx_config.carrier_en = true;
    rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);

    return true;
}

bool checkOUTIO() // Not used - looks like some voltage  check

{
    uint32_t sumadc1 = 0, sumadc2 = 0;
    pinMode(26, INPUT);

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

    if (sumadc1 < 3000)
    {
        ErrorMeg(0x61, "5V or 26 error");
    }
    if (sumadc2 < 2500)
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
        if ((tmp > 50) || (tmp < -20) || (hum > 100) || (hum == 0))
        {
            count++;
            if (count > 5)
            {
                ErrorMeg(0x91, "Grove error");
                return false;
            }
        }
    }
    return true;
}

void CheckHardware()
{
    if (InitI2SMicroPhone() != true)
    {
        ErrorMeg(0x51, "MicroPhone error");
    }
    if (M5.MPU6886.Init() != 0)
    {
        ErrorMeg(0x31, "MPU6886 error ");
    }
    if (InitIRTx() != true)
    {
        ErrorMeg(0x72, "RMT Init error");
    }
    if (InitBLEServer() != true)
    {
        ErrorMeg(0x81, "BLE init error");
    }
    checkAXP192();
    checkBM8563();
}
