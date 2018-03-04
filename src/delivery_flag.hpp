#pragma once

class DeliveryFlag {
public:
    void load();

    void reset();
    operator bool();

private:
    bool _value = false;
};