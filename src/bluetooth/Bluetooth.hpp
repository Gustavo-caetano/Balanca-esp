#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include "BluetoothSerial.h"
#include "ArduinoJson.h"

class Bluetooth{
  private:
    BluetoothSerial SerialBT;
    unsigned long tempo;
    bool connected;

  public:
    Bluetooth();
    void iniciar(const char* name);
    bool hasClient();
    void resetTime();
    void sendData(float peso, bool ativo);
    void sendMsg(String msg);
    void checkConnection();
    bool isConnected();
    bool connect(const char* name);
    bool disconnect();
    void descarga();
};

#endif // BLUETOOTH_HPP
