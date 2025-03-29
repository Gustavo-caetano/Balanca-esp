#include "Arduino.h"

#include <vector>

#include "ArduinoJson.h"

#include "utils/stringutils/StringUtils.hpp"

#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi.hpp"
#include "bluetooth/Bluetooth.hpp"
#include "eeprom/Eeprom.hpp"

const int pinoIgnicao = 2;
const String namespaceEeprom = "configs";

void menu(String opcao);
void menuBluetooth(String opcao);
void ignicao(void *par);
void printMenu();

String inputWifiBluetooth();
String inputWebsocketBluetooth();


Socket socket;
Balanca balanca;
Wifi wifi;
Bluetooth bluetooth;
Eeprom eeprom;


void setup()
{

  pinMode(pinoIgnicao, OUTPUT);

  Serial.begin(115200);
  Serial.println("Inicializando...");
  balanca.iniciar();

  bluetooth.iniciar("MASTER",menuBluetooth, printMenu);
  bluetooth.onMessageThread();

  eeprom.iniciar(namespaceEeprom);

  wifi.init(eeprom.getWifi());

  socket.iniciar(eeprom.getWebsocketServer());

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
  int indexSelecionado;

  switch (opcao.toInt())
  {
  case 0:
    ESP.restart();
    break;
  case 1:
    xTaskCreate(ignicao, "ignicao", 1000, NULL, 1, NULL);
    break;
  case 2:
    bluetooth.sendMsg(eeprom.getWifis());
    indexSelecionado = bluetooth.receiveString("informe o index desejado").toInt();

    eeprom.setWifi(indexSelecionado, inputWifiBluetooth());

    break;
  case 3:
    bluetooth.sendMsg(eeprom.getWebsocketServers());
    indexSelecionado = bluetooth.receiveString("informe o index desejado").toInt();

    eeprom.setWebsockerServer(indexSelecionado, inputWebsocketBluetooth());
    break;
  case 4:
    bluetooth.sendMsg("Index padrao: "+ String(eeprom.getIndexPadrao()));

    indexSelecionado = bluetooth.receiveString("informe o index desejado").toInt();

    eeprom.setIndexPadrao(indexSelecionado);

    break;
  case 9:
    printMenu();
    break;    

  default:
    bluetooth.sendMsg("comando invalido: " + opcao);
    break;
  }
}

void printMenu()
{
  bluetooth.sendMsg("Menu de opcoes do bluetooth");
  bluetooth.sendMsg("0 - Da restart no esp (util para aplicar configuracoes)");
  bluetooth.sendMsg("1 - realizar ignicao manual");
  bluetooth.sendMsg("2 - Configurar Wifi");
  bluetooth.sendMsg("3 - Configurar WebSocket");
  bluetooth.sendMsg("4 - Informar a config padrao");
}

String inputWifiBluetooth()
{
  
  std::vector <String> wifi;

  wifi.push_back(bluetooth.receiveString("informe o ssid do wifi"));
  wifi.push_back(bluetooth.receiveString("informe a senha do wifi"));

  String newWifi = StringUtils::join(wifi, '/');

  return newWifi;
}

String inputWebsocketBluetooth()
{

  String newWebSocket = bluetooth.receiveString("informe a url do websocket");

  return newWebSocket;
}