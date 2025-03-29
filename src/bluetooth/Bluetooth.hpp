#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include "BluetoothSerial.h"
#include "ArduinoJson.h"

class Bluetooth{
  private:
    BluetoothSerial SerialBT;
    unsigned long tempo;
    bool connected;
    void (*funcao)(std::string opcao);

    void (*printMenu)();
    static void onMessage(void *pvParameters);

  public:
    Bluetooth();
    void iniciar(const char* name, void (*menu)(std::string opcao),void (*printmenu)(), const bool isMaster = false);
    bool hasClient();
    void resetTime();
    void sendData(float peso, bool ativo);
    void sendMsg(std::string msg);
    std::string receiveString(std::string msg);
    void checkConnection();
    bool isConnected();
    bool connect(const char* name);
    bool disconnect();
    void descarga();
    void onMessageThread();
};

#endif // BLUETOOTH_HPP
