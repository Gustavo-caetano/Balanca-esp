#include "Eeprom.hpp"
#include "../utils/stringutils/StringUtils.hpp"

void Eeprom::iniciar(std::string nameSpace) {
    preferences.begin(nameSpace.c_str(), false);
}

int Eeprom::getIndexPadrao() {
    int index = preferences.getInt("index", 0);
    return index;
}

void Eeprom::setIndexPadrao(int index) {
    preferences.putInt("index", index);
}

std::string Eeprom::getWifis()
{
    std::string wifiData = preferences.getString("wifi", "").c_str();
    std::vector<std::string> wifis = StringUtils::split(wifiData);
    std::string wifiDataJoined = StringUtils::join(wifis, '\n');

    return wifiDataJoined;
}

std::string Eeprom::getWebsocketServers()
{
    std::string socketData = preferences.getString("socket", "").c_str();
    std::vector<std::string> sockets = StringUtils::split(socketData);
    std::string wifiDataJoined = StringUtils::join(sockets, '\n');

    return wifiDataJoined;
}

std::vector<std::string> Eeprom::getWifi() {
    int indexWifiPadrao = this->getIndexPadrao();
    std::string wifiData = preferences.getString("wifi", "").c_str();
    if (wifiData.length() == 0) {
        Serial.println("vazio o wifi");
        return {"", ""};
    }
    std::vector<std::string> wifis = StringUtils::split(wifiData);
    if (indexWifiPadrao >= 0 && indexWifiPadrao < wifis.size()) {
        
        return StringUtils::split(wifis[indexWifiPadrao], '/');
    } else {
        Serial.println("index incorreto");
        return {"", ""};
    }
}

bool Eeprom::setWifi(int index, std::string newWifi) {
    try {
        std::string wifiData = preferences.getString("wifi", "").c_str();
        std::vector<std::string> wifis = StringUtils::split(wifiData);
        if (index >= 0 && index < wifis.size()) {
            wifis[index] = newWifi;
            preferences.putString("wifi", StringUtils::join(wifis).c_str());
            return true;
        }
        else if (wifis.size() == index && index < 3) {
            wifis.push_back(newWifi);
            preferences.putString("wifi", StringUtils::join(wifis).c_str());
            return true;
        }
    } catch (...) {
        Serial.println("Erro ao definir WiFi");
    }
    return false;
}


std::string Eeprom::getWebsocketServer() {
    int indexSocketPadrao = this->getIndexPadrao();
    std::string socketData = preferences.getString("socket", "").c_str();
    if (socketData.length() == 0) {
        return socketData;
    }
    std::vector<std::string> sockets = StringUtils::split(socketData);
    if (indexSocketPadrao >= 0 && indexSocketPadrao < sockets.size()) {
        return sockets[indexSocketPadrao];
    } else {
        return socketData;
    }
}

bool Eeprom::setWebsocketServer(int index, std::string newWebsocket) {
    try {
        std::string socketData = preferences.getString("socket", "").c_str();
        std::vector<std::string> sockets = StringUtils::split(socketData);
        if (index >= 0 && index < sockets.size()) {
            sockets[index] = newWebsocket;
            preferences.putString("socket", StringUtils::join(sockets).c_str());
            return true;
        }
        else if (sockets.size() == index && index < 3) {
            sockets.push_back(newWebsocket);
            preferences.putString("socket", StringUtils::join(sockets).c_str());
            return true;
        }
        else {
            Serial.println("Index invalido");
        }
    } catch (...) {
        Serial.println("Erro ao definir WebSocket Server");
    }
    return false;
}


std::string Eeprom::getWebsocketRoom() {
    std::string socketDataRoom = preferences.getString("room", "").c_str();
    return socketDataRoom;
}

bool Eeprom::setWebsocketRoom(std::string newWebsocketRoom) {
    try {
        preferences.putString("room", newWebsocketRoom.c_str());
        return true;
    } catch (...) {
        Serial.println("Erro ao definir WebSocket Server");
    }
    return false;
}


float Eeprom::getNumberCalibration()
{
    float numberCalibration = preferences.getFloat("calibration", 277560.00);

    return numberCalibration;
}

bool Eeprom::setNumberCalibration(float numberCalibration)
{
    try {
        preferences.putFloat("calibration", numberCalibration);
        return true;
    } catch(...) {
        return false;
    }
}

bool Eeprom::getstandalone()
{
    bool standalone = preferences.getBool("standalone", true);
     
    return standalone;
}

bool Eeprom::setstandalone(bool standalone)
{
    try {
        preferences.putBool("standalone", standalone);
        return true;
    } catch(...) {
        return false;
    }
}