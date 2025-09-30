#include "Arduino.h"
#include <vector>
#include "ArduinoJson.h"
#include "utils/stringutils/StringUtils.hpp"
#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi_ESP.hpp"
#include "bluetooth/Bluetooth.hpp"
#include "eeprom/Eeprom.hpp"
#include "ota/OtaUpdate.hpp"

// Definições e Constantes
const int PINO_IGNICAO = 2;
const std::string NAMESPACE_EEPROM = "configs";

//variaveis usadas
bool standalone;

// Objetos Globais
Socket socket;
Balanca balanca;
Wifi wifi;
Bluetooth bluetooth;
Eeprom eeprom;
OtaUpdate otaUpdate;

// Declaração de Funções
void menu(std::string opcao);
void menuBluetooth(std::string opcao);
void ignicao(void *par);
void printMenu();
std::string inputWifiBluetooth();
std::string inputWebsocketBluetooth();
std::string inputWebsocketRoomBluetooth();
float inputCalibration();
bool configurarWifi();
bool configurarWebsocket();
bool configurarCalibracao();
bool configurarStandalone();
bool configurarRoom();
void handleIgnicao();
bool stringToBool(std::string str);
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

void setup() {
    pinMode(PINO_IGNICAO, OUTPUT);
    Serial.begin(115200);
    Serial.println("Inicializando...");
    eeprom.iniciar(NAMESPACE_EEPROM);
    
    std::string version = eeprom.getVersion();

    if(version == "") {
        eeprom.setVersion(VERSIONFIRMWARE);
        version = VERSIONFIRMWARE;
    }

    wifi.init(eeprom.getWifi(), standalone);

    otaUpdate.iniciar("https://rocket.guugascode.site");

    std::string versionUpdate = otaUpdate.atualizarHTTP(version);
    if(versionUpdate != "") {
        eeprom.setVersion(versionUpdate);
        ESP.restart();
    }
    
    standalone = eeprom.getstandalone();

    balanca.iniciar(eeprom.getNumberCalibration());

    bluetooth.iniciar("MASTER", menuBluetooth, btCallback);
    bluetooth.onMessageThread();
    socket.iniciar(eeprom.getWebsocketServer(), eeprom.getWebsocketRoom(), standalone);
    socket.onMenssage(menu);

}

void loop() {
    if(standalone) {
        delay(20);
        return;
    }

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
    vTaskDelete(NULL);
}

void handleIgnicao() {
    xTaskCreate(ignicao, "ignicao", 1024, NULL, 1, NULL);
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
            eeprom.setNumberCalibration(balanca.getScale());
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
        case 6:
            if(configurarStandalone())
            {
                bluetooth.sendMsg("Modo standalone configurado com sucesso");
                ESP.restart();
            }else {
                bluetooth.sendMsg("Erro ao configurar standalone");
            }
        case 7:
            if(configurarRoom())
            {
                bluetooth.sendMsg("Sala Websocket configurado com sucesso");
                ESP.restart();
            }else {
                bluetooth.sendMsg("Erro ao configurar a sala");
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
    return eeprom.setWebsocketServer(indexSelecionado, inputWebsocketBluetooth());
}

bool configurarCalibracao()
{
    bluetooth.sendMsg(std::to_string(eeprom.getNumberCalibration()));
    float calibration = inputCalibration();

    balanca.setScale(calibration);
    return eeprom.setNumberCalibration(balanca.getScale());
}

bool configurarStandalone()
{
    std::string msg = "para configurar o modo standalone preencha: \n 0(false) ou 1(true)\nOu escreva diretamente true ou false\n\n standalone = " + std::to_string(eeprom.getstandalone());

    bluetooth.sendMsg(msg);
    bool standalone = stringToBool(bluetooth.receiveString("\nInforme o valor"));
    Serial.println(standalone);

    return eeprom.setstandalone(standalone);
}

bool configurarRoom()
{
    std::string msg = "Opção de configurar sala \n id da sala atual: " + eeprom.getWebsocketRoom();
    bluetooth.sendMsg(msg);
    
    return eeprom.setWebsocketRoom(inputWebsocketRoomBluetooth());
}

void printMenu() {
    bluetooth.sendMsg(
    "Menu de opcoes do bluetooth\n"
    "0 - Reiniciar o ESP\n"
    "1 - Acionar ignicao manual\n"
    "2 - Configurar WiFi\n"
    "3 - Configurar WebSocket\n"
    "4 - Informar a config padrao\n"
    "5 - Configurar calibracao\n"
    "6 - Configurar modo standalone\n"
    "7 - Configurar room webSocket"
    );
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

std::string inputWebsocketRoomBluetooth() {
    return bluetooth.receiveString("Informe a Sala do WebSocket");
}

float inputCalibration()
{
    return std::stof(bluetooth.receiveString("Incremente ou decremente um valor"));
}

void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    printMenu();
  }
}


bool stringToBool(std::string str) {
  str.erase(0, str.find_first_not_of(" \t\n\r"));
  str.erase(str.find_last_not_of(" \t\n\r") + 1);
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);

  if (str == "true" || str == "1" || str == "yes" || str == "on") return true;
  if (str == "false" || str == "0" || str == "no" || str == "off") return false;

  Serial.println("Aviso: valor inválido para bool -> " + String(str.c_str()));
  return false;
}