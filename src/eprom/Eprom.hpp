#ifndef EPROM_HPP
#define EPROM_HPP

#include <Arduino.h>
#include <EEPROM.h>
#include <vector>
#include <string>

class Eprom {
public:
    void addWiFi(const String& ssid, const String& password);
    void removeWiFi(int index);
    std::vector<String> readWiFiList();
    void clearEEPROM();

private:
    static const int maxWiFiCount = 10;
    static const int ssidMaxLength = 32;
    static const int passwordMaxLength = 32;
    int getWiFiStartAddress(int index);
};

#endif // EPROM_HPP
