#include "Arduino.h"
#include <vector>
#include "ArduinoJson.h"
#include "utils/stringutils/StringUtils.hpp"
#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi.hpp"
#include "bluetooth/Bluetooth.hpp"
#include "eeprom/Eeprom.hpp"

// Definições e Constantes
const int PINO_IGNICAO = 2;
const String NAMESPACE_EEPROM = "configs";

// Objetos Globais
Socket socket;
Balanca balanca;
Wifi wifi;
Bluetooth bluetooth;
Eeprom eeprom;

// Declaração de Funções
void menu(String opcao);
void menuBluetooth(String opcao);
void ignicao(void *par);
void printMenu();
String inputWifiBluetooth();
String inputWebsocketBluetooth();
bool configurarWifi();
bool configurarWebsocket();
void handleIgnicao();

void setup() {
    pinMode(PINO_IGNICAO, OUTPUT);
    Serial.begin(115200);
    Serial.println("Inicializando...");
    
    balanca.iniciar();
    bluetooth.iniciar("MASTER", menuBluetooth, printMenu);
    bluetooth.onMessageThread();
    eeprom.iniciar(NAMESPACE_EEPROM);
    
    wifi.init(eeprom.getWifi());
    socket.iniciar(eeprom.getWebsocketServer());
    socket.onMenssage(menu);
}

void loop() {
    socket.checkConnection();
    if (socket.isConnected()) {
        socket.poll();
    }
    
    float peso = balanca.getPeso();
    socket.sendData(peso, balanca.isActive());
    delay(20);
}

void ignicao(void *par) {
    digitalWrite(PINO_IGNICAO, HIGH);
    Serial.println("Ignicao acionada");
    vTaskDelay(pdMS_TO_TICKS(5000));
    digitalWrite(PINO_IGNICAO, LOW);
}

void handleIgnicao() {
    xTaskCreate(ignicao, "ignicao", 1000, NULL, 1, NULL);
}

void menu(String data) {
    JsonDocument json;
    deserializeJson(json, data);

    int opcao = json["Opcao"];
    float valor;

    switch (opcao) {
        case 1:
            valor = json["Valor"];
            balanca.setScale(valor);
            break;
        case 2:
            balanca.tara();
            break;
        case 3:
            handleIgnicao();
            break;
        default:
            Serial.println("Opcao invalida");
            break;
    }
}

void menuBluetooth(String opcao) {
    int indexSelecionado;

    switch (opcao.toInt()) {
        case 0:
            ESP.restart();
            break;
        case 1:
            handleIgnicao();
            break;
        case 2:
            if (configurarWifi()) {
                bluetooth.sendMsg("WiFi configurado com sucesso");
            } else {
                bluetooth.sendMsg("Erro ao configurar o WiFi");
            }
            break;
        case 3:
            if (configurarWebsocket()) {
                bluetooth.sendMsg("WebSocket configurado com sucesso");
            } else {
                bluetooth.sendMsg("Erro ao configurar o WebSocket");
            }
            break;
        case 4:
            bluetooth.sendMsg("Index padrao: " + String(eeprom.getIndexPadrao()));
            indexSelecionado = bluetooth.receiveString("Informe o index desejado").toInt();
            eeprom.setIndexPadrao(indexSelecionado);
            break;
        case 9:
            printMenu();
            break;
        default:
            bluetooth.sendMsg("Comando invalido: " + opcao);
            break;
    }
}

bool configurarWifi() {
    bluetooth.sendMsg(eeprom.getWifis());
    int indexSelecionado = bluetooth.receiveString("Informe o index desejado").toInt();
    return eeprom.setWifi(indexSelecionado, inputWifiBluetooth());
}

bool configurarWebsocket() {
    bluetooth.sendMsg(eeprom.getWebsocketServers());
    int indexSelecionado = bluetooth.receiveString("Informe o index desejado").toInt();
    return eeprom.setWebsockerServer(indexSelecionado, inputWebsocketBluetooth());
}

void printMenu() {
    bluetooth.sendMsg("Menu de opcoes do bluetooth");
    bluetooth.sendMsg("0 - Reiniciar o ESP");
    bluetooth.sendMsg("1 - Acionar ignicao manual");
    bluetooth.sendMsg("2 - Configurar WiFi");
    bluetooth.sendMsg("3 - Configurar WebSocket");
    bluetooth.sendMsg("4 - Informar a config padrao");
}

String inputWifiBluetooth() {
    std::vector<String> wifi;
    wifi.push_back(bluetooth.receiveString("Informe o SSID do WiFi"));
    wifi.push_back(bluetooth.receiveString("Informe a senha do WiFi"));
    return StringUtils::join(wifi, '/');
}

String inputWebsocketBluetooth() {
    return bluetooth.receiveString("Informe a URL do WebSocket");
}
