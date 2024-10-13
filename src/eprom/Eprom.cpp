#include "EPROM.hpp"

void Eprom::addWiFi(const String& ssid, const String& password) {
    for (int i = 0; i < maxWiFiCount; ++i) {
        if (EEPROM.read(getWiFiStartAddress(i)) == 0) {
            for (int j = 0; j < ssidMaxLength; ++j) {
                EEPROM.write(getWiFiStartAddress(i) + 1 + j, j < ssid.length() ? ssid[j] : 0);
            }
            for (int j = 0; j < passwordMaxLength; ++j) {
                EEPROM.write(getWiFiStartAddress(i) + 1 + ssidMaxLength + j, j < password.length() ? password[j] : 0);
            }
            EEPROM.write(getWiFiStartAddress(i), 1);
            EEPROM.commit();
            break;
        }
    }
}

std::vector<String> Eprom::readWiFiList() {
    std::vector<String> wifiList;
    for (int i = 0; i < maxWiFiCount; ++i) {
        if (EEPROM.read(getWiFiStartAddress(i)) == 1) {
            String ssid, password;
            for (int j = 0; j < ssidMaxLength; ++j) {
                char c = EEPROM.read(getWiFiStartAddress(i) + 1 + j);
                if (c == 0) break;
                ssid += c;
            }
            for (int j = 0; j < passwordMaxLength; ++j) {
                char c = EEPROM.read(getWiFiStartAddress(i) + 1 + ssidMaxLength + j);
                if (c == 0) break;
                password += c;
            }
            wifiList.push_back(ssid + " : " + password);
        }
    }
    return wifiList;
}

void Eprom::clearEEPROM() {
    for (int i = 0; i < maxWiFiCount; ++i) {
        EEPROM.write(getWiFiStartAddress(i), 0);
    }
}

int Eprom::getWiFiStartAddress(int index) {
    return index * (1 + ssidMaxLength + passwordMaxLength);
}

void Eprom::removeWiFi(int index) {
    for (int i = 0; i < maxWiFiCount; ++i) {
        if (EEPROM.read(getWiFiStartAddress(i)) == 1) {
            String storedSSID;
            for (int j = 0; j < ssidMaxLength; ++j) {
                char c = EEPROM.read(getWiFiStartAddress(i) + 1 + j);
                if (c == 0) break;
                storedSSID += c;
            }
            
            if (i == index - 1) {
                EEPROM.write(getWiFiStartAddress(i), 0);
                EEPROM.commit();
                break;
            }
        }
    }
}
