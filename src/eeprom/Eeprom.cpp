#include "Eeprom.hpp"
#include "../utils/stringutils/StringUtils.hpp"

void Eeprom::iniciar(String nameSpace) {
    preferences.begin(nameSpace.c_str(), false);
}

int Eeprom::getIndexPadrao() {
    int index = preferences.getInt("index", 0);
    return index;
}

void Eeprom::setIndexPadrao(int index) {
    preferences.putInt("index", index);
}

String Eeprom::getWifis()
{
    String wifiData = preferences.getString("wifi", "");
    std::vector<String> wifis = StringUtils::split(wifiData);
    String wifiDataJoined = StringUtils::join(wifis, '\n');

    return wifiDataJoined;
}

String Eeprom::getWebsocketServers()
{
    String socketData = preferences.getString("socket", "");
    std::vector<String> sockets = StringUtils::split(socketData);
    String wifiDataJoined = StringUtils::join(sockets, '\n');

    return wifiDataJoined;
}

std::vector<String> Eeprom::getWifi() {
    int indexWifiPadrao = this->getIndexPadrao();
    String wifiData = preferences.getString("wifi", "");
    if (wifiData.isEmpty()) {
        Serial.println("vazio o wifi");
        return {"", ""};
    }
    std::vector<String> wifis = StringUtils::split(wifiData);
    if (indexWifiPadrao >= 0 && indexWifiPadrao < wifis.size()) {
        
        return StringUtils::split(wifis[indexWifiPadrao], '/');
    } else {
        Serial.println("index incorreto");
        return {"", ""};
    }
}

void Eeprom::setWifi(int index, String newWifi) {
    String wifiData = preferences.getString("wifi", "");
    std::vector<String> wifis = StringUtils::split(wifiData);
    if (index >= 0 && index < wifis.size()) {
        wifis[index] = newWifi;
        preferences.putString("wifi", StringUtils::join(wifis));
    }
    else if(wifis.size() == index && index < 3)
    {
        wifis.push_back(newWifi);
        preferences.putString("wifi", StringUtils::join(wifis));
    }
}

String Eeprom::getWebsocketServer() {
    int indexSocketPadrao = this->getIndexPadrao();
    String socketData = preferences.getString("socket", "");
    if (socketData.isEmpty()) {
        return socketData;
    }
    std::vector<String> sockets = StringUtils::split(socketData);
    if (indexSocketPadrao >= 0 && indexSocketPadrao < sockets.size()) {
        return sockets[indexSocketPadrao];
    } else {
        return socketData;
    }
}

void Eeprom::setWebsockerServer(int index, String newWebsocket) {
    String socketData = preferences.getString("socket", "");
    std::vector<String> sockets = StringUtils::split(socketData);
    if (index >= 0 && index < sockets.size()) {
        sockets[index] = newWebsocket;
        preferences.putString("socket", StringUtils::join(sockets));
    }
    else if(sockets.size() == index && index < 3)
    {
        sockets.push_back(newWebsocket);
        preferences.putString("socket", StringUtils::join(sockets));
    }
    else
    {
        Serial.println("Index invalido");
    }
}
