#include "Bluetooth.hpp"

Bluetooth::Bluetooth(void (*menu)(String opcao),void (*printmenu)(String opcao)) 
  : tempo(0), connected(false), funcao(menu), printMenu(printmenu) {}

void Bluetooth::iniciar(const char* name, const bool isMaster = false) {
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
  StaticJsonDocument<200> data;
  String jsonString;

  data["Tempo"] = millis() - tempo;
  data["Peso"] = peso;
  data["Ativo"] = ativo;

  serializeJson(data, jsonString);
  SerialBT.println(jsonString);
  Serial.println(jsonString);
}

void Bluetooth::sendMsg(String msg)
{
  if(SerialBT.connected())
  {
    SerialBT.println(msg);
    disconnect();
  }
  else {
    Serial.println("nao esta conectado");
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
      String dado = String(self->SerialBT.read());
      
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
    1024,
    this,
    1,
    NULL,
    1   
  );
}