#include "Bluetooth.hpp"

Bluetooth::Bluetooth() : tempo(0), connected(false) {}

void Bluetooth::iniciar(const char* name) {
  SerialBT.begin(name);
  Serial.println("Bluetooth iniciado, aguardando conexão...");
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
  SerialBT.println(msg);
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
  SerialBT.connect(name);
}