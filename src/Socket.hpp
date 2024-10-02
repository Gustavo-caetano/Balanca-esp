#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "ArduinoWebsockets.h"
#include "ArduinoJson.h"
#include <WiFi.h>

class Socket{
  private:
    websockets::WebsocketsClient webSocket;
    unsigned long tempo;
    bool connected;
    String ssid;
    String passwd;
    String host;

  public:
    Socket();
    void iniciar(String  ssid, String  password, String  server);
    bool hasClient();
    void resetTime();
    void sendData(float peso, bool ativo);
    void checkConnection();
    bool isConnected();
    bool connectWIFI();
    bool connectSK();
};

#endif // BLUETOOTH_HPP
