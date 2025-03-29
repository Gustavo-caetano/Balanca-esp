#include "Bluetooth.hpp"

#include "../utils/stringutils/StringUtils.hpp"

Bluetooth::Bluetooth() 
  : tempo(0), connected(false) {}

void Bluetooth::iniciar(const char* name, void (*menu)(std::string opcao),void (*printmenu)(), const bool isMaster) {

  funcao = menu;
  printMenu = printmenu;

  SerialBT.begin(name, isMaster );
  Serial.println("Bluetooth iniciado");
}

bool Bluetooth::hasClient() {
  return SerialBT.hasClient();
}

void Bluetooth::resetTime() {
  tempo = millis();
}

void Bluetooth::sendData(float peso, bool ativo) {
  
  JsonDocument data;
  data.to<JsonObject>(); 
  std::string jsonString;

  data["Tempo"] = millis() - tempo;
  data["Peso"] = peso;
  data["Ativo"] = ativo;

  serializeJson(data, jsonString);
  SerialBT.println(jsonString.c_str());
  Serial.println(jsonString.c_str());
}

void Bluetooth::sendMsg(std::string msg)
{
  if(SerialBT.connected())
  {
    SerialBT.println(msg.c_str());
  }
  else {
    Serial.println("nao esta conectado");
  }
}

std::string Bluetooth::receiveString(std::string msg)
{
    if (SerialBT.connected())
    {
        SerialBT.println(msg.c_str());

        std::string dado;
        unsigned long tempoInicial = millis();

        while (millis() - tempoInicial < 50000)  
        {
            if (SerialBT.available())
            {
                dado = SerialBT.readStringUntil('\n').c_str();  
                return StringUtils::trim(dado);
            }
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }

        Serial.println("Tempo esgotado esperando resposta.");
        return "";
    }
    else 
    {
        Serial.println("Nao esta conectado");
        return "";
    }
}


void Bluetooth::checkConnection() {
  bool clienteConectado = hasClient();
  if (clienteConectado != connected) {
    Serial.println(clienteConectado ? "Dispositivo conectado via Bluetooth!" : "Dispositivo desconectado.");
    connected = clienteConectado;
    if (connected) {
      resetTime();
    }
  }
}

bool Bluetooth::isConnected() {
  return connected;
}

bool Bluetooth::connect(const char* name)
{
    Serial.println("Tentando conectar ao dispositivo Bluetooth...");
    
    if (name == nullptr || strlen(name) == 0) {
        Serial.println("Erro: Nome do dispositivo inválido.");
        return false;
    }
    
    try {
        if (SerialBT.connect(name)) {
            Serial.println("Conectado com sucesso!");
            return true;
        } else {
            Serial.println("Erro: Não foi possível conectar ao dispositivo.");
            return false;
        }
    } catch (const std::exception& e) {
        Serial.print("Erro: Ocorreu uma exceção durante a tentativa de conexão: ");
        Serial.println(e.what());
        return false;
    } catch (...) {
        Serial.println("Erro: Ocorreu uma exceção inesperada durante a tentativa de conexão.");
        return false;
    }
}

bool Bluetooth::disconnect()
{
  return SerialBT.disconnect();
}

void Bluetooth::descarga()
{
  while (SerialBT.available()) {
      Serial.println(SerialBT.read());
    }
}

void Bluetooth::onMessage(void *pvParameters)
{
  Bluetooth *self = static_cast<Bluetooth *>(pvParameters);

  while (true)
  {
    if(self->SerialBT.available())
    {
      std::string dado = self->SerialBT.readStringUntil('\n').c_str();  
      Serial.println("comando executado: " + String(dado.c_str()));
      self->funcao(dado);
    }

    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
  
}

void Bluetooth::onMessageThread()
{
  xTaskCreatePinnedToCore(
    Bluetooth::onMessage,
    "bluetoothMessage",
    4096,
    this,
    1,
    NULL,
    1   
  );
}