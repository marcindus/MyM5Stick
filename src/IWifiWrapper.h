#pragma once

class IWifiWrapper
{
public:
    virtual ~IWifiWrapper(){};
    virtual void set_promiscuous(bool) = 0;
    virtual void set_promiscuous_rx_cb(wifi_promiscuous_cb_t) = 0;
    virtual void set_channel(uint8_t) = 0;
};
