#include "ArduinoJson.h"

#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi.hpp"

Socket socket;
Balanca balanca;
Wifi wifi;

void menu(String opcao);

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando...");
  balanca.iniciar();

  wifi.init(
      "LUCAS",
      "Lucas65780213");

  socket.iniciar("ws://192.168.0.114:15000");
  socket.onMenssage(menu);
}

void loop()
{
  socket.checkConnection();
  if (socket.isConnected())
  {
    float peso = balanca.getPeso();
    socket.sendData(peso, balanca.isActive());
  }
  delay(20);
}

void menu(String data)
{
  Serial.println("mensagem recebida");
  JsonDocument json;
  deserializeJson(json, data);
  int opcao = json["opcao"];
  float valor;
  switch (opcao)
  {
  case 1:
    valor = json["valor"];
    balanca.setScale(valor);
    Serial.println("calibdrado!!");
    break;
  case 2:
    Serial.println("tara realizado!!");
    balanca.tara();
    break;
  default:
    break;
  }
}
