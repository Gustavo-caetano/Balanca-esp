#include "Eeprom.hpp"
#include "../utils/stringutils/StringUtils.hpp"

void Eeprom::iniciar(const std::string &nameSpace) {
    safeSet([&] { preferences.begin(nameSpace.c_str(), false); });
}

int Eeprom::getIndexPadrao() {
    return safeGet<int>([&] { return preferences.getInt("index", 0); }, 0);
}

void Eeprom::setIndexPadrao(int index) {
    safeSet([&] { preferences.putInt("index", index); });
}

std::string Eeprom::getWifis() {
    return safeGet<std::string>([&] {
        std::string wifiData = preferences.getString("wifi", "").c_str();
        auto wifis = StringUtils::split(wifiData);
        return StringUtils::join(wifis, '\n');
    }, "");
}

std::string Eeprom::getWebsocketServers() {
    return safeGet<std::string>([&] {
        std::string socketData = preferences.getString("socket", "").c_str();
        auto sockets = StringUtils::split(socketData);
        return StringUtils::join(sockets, '\n');
    }, "");
}

std::vector<std::string> Eeprom::getWifi() {
    return safeGet<std::vector<std::string>>([&] {
        int index = getIndexPadrao();
        std::string wifiData = preferences.getString("wifi", "").c_str();
        if (wifiData.empty()) {
            Serial.println("vazio o wifi");
            return std::vector<std::string>{"", ""};
        }
        auto wifis = StringUtils::split(wifiData);
        if (index >= 0 && index < (int)wifis.size()) {
            return StringUtils::split(wifis[index], '/');
        } else {
            Serial.println("index incorreto");
            return std::vector<std::string>{"", ""};
        }
    }, {"", ""});
}

bool Eeprom::setWifi(int index, const std::string &newWifi) {
    return safeSet([&] {
        std::string wifiData = preferences.getString("wifi", "").c_str();
        auto wifis = StringUtils::split(wifiData);

        if (index >= 0 && index < (int)wifis.size()) {
            wifis[index] = newWifi;
        } else if ((int)wifis.size() == index && index < 3) {
            wifis.push_back(newWifi);
        } else {
            throw std::runtime_error("Index inválido em setWifi");
        }

        preferences.putString("wifi", StringUtils::join(wifis).c_str());
    });
}

std::string Eeprom::getWebsocketServer() {
    return safeGet<std::string>([&] {
        int index = getIndexPadrao();
        std::string socketData = preferences.getString("socket", "").c_str();
        if (socketData.empty()) return socketData;

        auto sockets = StringUtils::split(socketData);
        if (index >= 0 && index < (int)sockets.size()) {
            return sockets[index];
        } else {
            return socketData;
        }
    }, "");
}

bool Eeprom::setWebsocketServer(int index, const std::string &newWebsocket) {
    return safeSet([&] {
        std::string socketData = preferences.getString("socket", "").c_str();
        auto sockets = StringUtils::split(socketData);

        if (index >= 0 && index < (int)sockets.size()) {
            sockets[index] = newWebsocket;
        } else if ((int)sockets.size() == index && index < 3) {
            sockets.push_back(newWebsocket);
        } else {
            throw std::runtime_error("Index inválido em setWebsocketServer");
        }

        preferences.putString("socket", StringUtils::join(sockets).c_str());
    });
}

std::string Eeprom::getWebsocketRoom() {
    return safeGet<std::string>([&] {
        return preferences.getString("room", "").c_str();
    }, "");
}

bool Eeprom::setWebsocketRoom(const std::string &newWebsocketRoom) {
    return safeSet([&] { preferences.putString("room", newWebsocketRoom.c_str()); });
}

float Eeprom::getNumberCalibration() {
    return safeGet<float>([&] { return preferences.getFloat("calibration", 277560.00); }, 277560.00);
}

bool Eeprom::setNumberCalibration(float numberCalibration) {
    return safeSet([&] { preferences.putFloat("calibration", numberCalibration); });
}

bool Eeprom::getstandalone() {
    return safeGet<bool>([&] { return preferences.getBool("standalone", true); }, true);
}

bool Eeprom::setstandalone(bool standalone) {
    return safeSet([&] { preferences.putBool("standalone", standalone); });
}

std::string Eeprom::getVersion() {
    return safeGet<std::string>([&] { return preferences.getString("version", "").c_str(); }, "");
}

bool Eeprom::setVersion(const std::string &version) {
    return safeSet([&] { preferences.putString("version", version.c_str()); });
}

bool Eeprom::getAutomaticUpdate() {
    return safeGet<bool>([&] { return preferences.getBool("automaticUpdate", false); }, false);
}

bool Eeprom::setAutomaticUpdate(bool automaticUpdate) {
    return safeSet([&] { preferences.putBool("automaticUpdate", automaticUpdate); });
}
