#pragma once

#include <gmock/gmock.h>
#include "IWifiWrapper.h"
#include "esp_wifi_types.h"
#include "esp_err.h"
#include "esp_wifi.h"
//#include "esp_wifi_types_private.h"

class WifiWrapperMock : public IWifiWrapper
{
public:

   MOCK_METHOD1(_esp_wifi_init, void(wifi_init_config_t) );
   MOCK_METHOD1(_esp_wifi_set_country, void(wifi_country_t));
   MOCK_METHOD1(_esp_wifi_set_storage, void(wifi_storage_t));
   MOCK_METHOD1(_esp_wifi_set_mode, void(wifi_mode_t));
   MOCK_METHOD0(_esp_wifi_start, void());
   MOCK_METHOD0(_esp_wifi_stop, void());
   MOCK_METHOD1(_esp_wifi_set_promiscuous, void(bool));
   MOCK_METHOD1(_esp_wifi_set_promiscuous_rx_cb, void(wifi_promiscuous_cb_t));
   MOCK_METHOD2(_esp_wifi_set_channel, void(uint8_t primary, wifi_second_chan_t secondary));
};

