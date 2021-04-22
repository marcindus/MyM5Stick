#pragma once

class ISniffer
{
public:
    virtual ~ISniffer(){};
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void on_packet() = 0;
    virtual void print_results() = 0;
    virtual uint8_t get_channel() = 0;
    virtual void set_channel(uint8_t) = 0;
};


