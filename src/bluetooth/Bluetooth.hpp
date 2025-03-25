#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include "BluetoothSerial.h"
#include "ArduinoJson.h"

class Bluetooth{
  private:
    BluetoothSerial SerialBT;
    unsigned long tempo;
    bool connected;
    void (*funcao)(String opcao);

    void (*printMenu)(String opcao);
    static void onMessage(void *pvParameters);

  public:
    Bluetooth(void (*menu)(String opcao), void (*printmenu)(String opcao));
    void iniciar(const char* name, const bool isMaster = false);
    bool hasClient();
    void resetTime();
    void sendData(float peso, bool ativo);
    void sendMsg(String msg);
    void checkConnection();
    bool isConnected();
    bool connect(const char* name);
    bool disconnect();
    void descarga();
    void onMessageThread();
};

#endif // BLUETOOTH_HPP
