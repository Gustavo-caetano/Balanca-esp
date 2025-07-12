#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "ArduinoWebsockets.h"
#include "ArduinoJson.h"

class Socket{
  private:
    websockets::WebsocketsClient webSocket;
    unsigned long tempo;
    bool connected;
    std::string host;

  public:
    Socket();
    void iniciar(std::string  server, bool standalone);
    bool hasClient();
    void resetTime();
    void sendData(float peso, bool ativo);
    void checkConnection();
    bool isConnected();
    bool connectSK();
    void onMenssage(void (*funcao)(std::string opcao));
    void poll();
};

#endif // BLUETOOTH_HPP
