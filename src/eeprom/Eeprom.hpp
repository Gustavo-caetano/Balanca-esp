#ifndef EEPROM_HPP
#define EEPROM_HPP

#include <Preferences.h>
#include <vector>

class Eeprom
{
private:
    Preferences preferences;
public:
    void iniciar(String nameSpace);

    int getIndexPadrao();
    void setIndexPadrao(int index);

    String getWifis();
    String getWebsocketServers();
    
    std::vector <String> getWifi();
    void setWifi(int index , String newWifi);

    String getWebsocketServer();
    void setWebsockerServer(int index, String newWebsocket);
};

#endif // EEPROM_HPP