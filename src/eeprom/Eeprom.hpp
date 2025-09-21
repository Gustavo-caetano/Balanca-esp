#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <Preferences.h>
#include <vector>

class Eeprom
{
private:
    Preferences preferences;
public:
    void iniciar(std::string nameSpace);

    int getIndexPadrao();
    void setIndexPadrao(int index);

    std::string getWifis();
    std::string getWebsocketServers();
    
    std::vector <std::string> getWifi();
    bool setWifi(int index , std::string newWifi);

    std::string getWebsocketServer();
    bool setWebsocketServer(int index, std::string newWebsocket);

    std::string getWebsocketRoom();
    bool setWebsocketRoom(std::string newWebsocketRoom);

    float getNumberCalibration();
    bool setNumberCalibration(float numberCalibration);

    bool getstandalone();
    bool setstandalone(bool standalone);
};

#endif // EEPROM_HPP