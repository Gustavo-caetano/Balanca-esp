#include "Arduino.h"

#include "ArduinoJson.h"

#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi.hpp"
#include "bluetooth/Bluetooth.hpp"

const int pinoIgnicao = 2;

Socket socket;
Balanca balanca;
Wifi wifi;
Bluetooth bluetooth;

void menu(String opcao);
void menuBluetooth(String opcao);
void ignicao(void *par);

void setup()
{

  pinMode(pinoIgnicao, OUTPUT);

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
      "ws://192.168.0.108:15000");

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

void ignicao(void *par)
{
  digitalWrite(pinoIgnicao, HIGH);
  
  Serial.println("acionado");
  vTaskDelay(pdMS_TO_TICKS(5000));

  digitalWrite(pinoIgnicao, LOW);
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
    xTaskCreate(ignicao, "ignicao", 1000, NULL, 1, NULL);
    break;
  default:
    break;
  }
}

void menuBluetooth(String opcao)
{
  switch (opcao.toInt())
  {
  case 1:
    xTaskCreate(ignicao, "ignicao", 1000, NULL, 1, NULL);
    break;
  case 2:
    ESP.restart();
    break;
    
  default:
    break;
  }
}