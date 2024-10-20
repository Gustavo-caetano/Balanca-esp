#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "ArduinoWebsockets.h"
#include "ArduinoJson.h"

class Socket{
  private:
    websockets::WebsocketsClient webSocket;
    unsigned long tempo;
    bool connected;
    String host;

  public:
    Socket();
    void iniciar(String  server);
    bool hasClient();
    void resetTime();
    void sendData(float peso, bool ativo);
    void checkConnection();
    bool isConnected();
    bool connectSK();
    void onMenssage(void (*funcao)(String opcao));
};

#endif // BLUETOOTH_HPP
