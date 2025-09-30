#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <Preferences.h>
#include <vector>
#include <string>

class Eeprom {
private:
    Preferences preferences;

    template<typename T, typename F>
    T safeGet(F func, T fallback) {
        try {
            return func();
        } catch (...) {
            Serial.print("Erro em "); Serial.println(__FUNCTION__);
            return fallback;
        }
    }

    template<typename F>
    bool safeSet(F func) {
        try {
            func();
            return true;
        } catch (...) {
            Serial.print("Erro em "); Serial.println(__FUNCTION__);
            return false;
        }
    }

public:
    void iniciar(const std::string &nameSpace);

    int getIndexPadrao();
    void setIndexPadrao(int index);

    std::string getWifis();
    std::string getWebsocketServers();

    std::vector<std::string> getWifi();
    bool setWifi(int index, const std::string &newWifi);

    std::string getWebsocketServer();
    bool setWebsocketServer(int index, const std::string &newWebsocket);

    std::string getWebsocketRoom();
    bool setWebsocketRoom(const std::string &newWebsocketRoom);

    float getNumberCalibration();
    bool setNumberCalibration(float numberCalibration);

    bool getstandalone();
    bool setstandalone(bool standalone);

    std::string getVersion();
    bool setVersion(const std::string &version);

    bool getAutomaticUpdate();
    bool setAutomaticUpdate(bool automaticUpdate);
};

#endif // EEPROM_HPP
