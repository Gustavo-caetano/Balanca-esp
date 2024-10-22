#include "ArduinoJson.h"

#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi.hpp"
#include "bluetooth/Bluetooth.hpp"

Socket socket;
Balanca balanca;
Wifi wifi;
Bluetooth bluetooth;

void menu(String opcao);

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando...");
  balanca.iniciar();

  // wifi.init(
  //     "PROJETO1",
  //     "umdoistres");

  wifi.init(
      "LUCAS",
      "Lucas65780213");

  bluetooth.iniciar("MASTER");

  socket.iniciar(
      "ws://192.168.0.114:15000");

  socket.onMenssage(menu);
}

void loop()
{
  socket.checkConnection();
  if (socket.isConnected())
  {
    socket.poll();
  }
  float peso = balanca.getPeso();
  socket.sendData(peso, balanca.isActive());
  delay(20);
}

void menu(String data)
{
  JsonDocument json;
  deserializeJson(json, data);

  int opcao = json["Opcao"];
  float valor;

  switch (opcao)
  {
  case 1:
    valor = json["Valor"];
    balanca.setScale(valor);
    break;
  case 2:
    balanca.tara();
    break;
  case 3:
    bluetooth.connect("FOGO_BT");
    bluetooth.sendMsg("b");
    Serial.println("Ignicao acionada!!");
    break;
  default:
    break;
  }
}