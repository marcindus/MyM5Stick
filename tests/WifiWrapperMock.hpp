#pragma once

#include <gmock/gmock.h>
#include "IWifiWrapper.h"

class WifiWrapperMock : public IWifiWrapper
{
public:

   MOCK_METHOD0(_esp_wifi_init, void() );
   MOCK_METHOD0(_esp_wifi_set_country, void());
   MOCK_METHOD0(_esp_wifi_set_storage, void());
   MOCK_METHOD0(_esp_wifi_set_mode, void());
   MOCK_METHOD0(_esp_wifi_start, void());
   MOCK_METHOD0(_esp_wifi_stop, void());
   MOCK_METHOD1(_esp_wifi_set_promiscuous, void(bool));
   MOCK_METHOD0(_esp_wifi_set_promiscuous_rx_cb, void());
   MOCK_METHOD1(_esp_wifi_set_channel, void(uint8_t));
};

