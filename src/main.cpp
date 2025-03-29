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
const std::string NAMESPACE_EEPROM = "configs";

// Objetos Globais
Socket socket;
Balanca balanca;
Wifi wifi;
Bluetooth bluetooth;
Eeprom eeprom;

// Declaração de Funções
void menu(std::string opcao);
void menuBluetooth(std::string opcao);
void ignicao(void *par);
void printMenu();
std::string inputWifiBluetooth();
std::string inputWebsocketBluetooth();
float inputCalibration();
bool configurarWifi();
bool configurarWebsocket();
bool configurarCalibracao();
void handleIgnicao();

void setup() {
    pinMode(PINO_IGNICAO, OUTPUT);
    Serial.begin(115200);
    Serial.println("Inicializando...");
    eeprom.iniciar(NAMESPACE_EEPROM);
    
    balanca.iniciar(eeprom.getNumberCalibration());
    bluetooth.iniciar("MASTER", menuBluetooth, printMenu);
    bluetooth.onMessageThread();
    
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

void menu(std::string data) {
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

void menuBluetooth(std::string opcao) {
    int indexSelecionado;

    switch (std::stoi(opcao)) {
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
            bluetooth.sendMsg("Index padrao: " + eeprom.getIndexPadrao());
            indexSelecionado = std::stoi(bluetooth.receiveString("Informe o index desejado"));
            eeprom.setIndexPadrao(indexSelecionado);
            break;
        case 5:
            if(configurarCalibracao())
            {
                bluetooth.sendMsg("Calibracao configurada com sucesso");
            }else {
                bluetooth.sendMsg("Erro ao configurar a calibracao");
            }
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
    int indexSelecionado = std::stoi(bluetooth.receiveString("Informe o index desejado"));
    return eeprom.setWifi(indexSelecionado, inputWifiBluetooth());
}

bool configurarWebsocket() {
    bluetooth.sendMsg(eeprom.getWebsocketServers());
    int indexSelecionado = std::stoi(bluetooth.receiveString("Informe o index desejado"));
    return eeprom.setWebsockerServer(indexSelecionado, inputWebsocketBluetooth());
}

bool configurarCalibracao()
{
    bluetooth.sendMsg(std::to_string(eeprom.getNumberCalibration()));
    float calibration = inputCalibration();

    balanca.setScale(calibration);
    return eeprom.setNumberCalibration(balanca.getScale());
}

void printMenu() {
    bluetooth.sendMsg("Menu de opcoes do bluetooth");
    bluetooth.sendMsg("0 - Reiniciar o ESP");
    bluetooth.sendMsg("1 - Acionar ignicao manual");
    bluetooth.sendMsg("2 - Configurar WiFi");
    bluetooth.sendMsg("3 - Configurar WebSocket");
    bluetooth.sendMsg("4 - Informar a config padrao");
    bluetooth.sendMsg("5 - Configurar calibracao");
}

std::string inputWifiBluetooth() {
    std::vector<std::string> wifi;
    wifi.push_back(bluetooth.receiveString("Informe o SSID do WiFi"));
    wifi.push_back(bluetooth.receiveString("Informe a senha do WiFi"));
    return StringUtils::join(wifi, '/');
}

std::string inputWebsocketBluetooth() {
    return bluetooth.receiveString("Informe a URL do WebSocket");
}

float inputCalibration()
{
    return std::stof(bluetooth.receiveString("Incremente ou decremente um valor"));
}