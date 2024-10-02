#include "Socket.hpp"
#include <WiFi.h>

Socket::Socket() : tempo(0), connected(false) {}

void Socket::iniciar(String ssid, String password, String server)
{
    this->ssid = ssid;
    this->passwd = password;
    this->host = server;

    connectWIFI();
    connectSK();
}

bool Socket::hasClient()
{
    return connected;
}

void Socket::resetTime()
{
    tempo = millis();
}

void Socket::sendData(float peso, bool ativo)
{
    if (connected)
    {
        DynamicJsonDocument doc(1024);
        doc["Tempo"] = millis() - tempo;
        doc["Peso"] = peso;
        doc["Ativo"] = ativo;

        String output;
        serializeJson(doc, output);

        if (webSocket.send(output))
        {
            Serial.println("Dados enviados: " + output);
        }
        else
        {
            Serial.println("Falha ao enviar dados!");
        }
    }
    else
    {
        Serial.println("WebSocket não está conectado. Não foi possível enviar dados.");
    }
}

void Socket::checkConnection()
{
    bool clienteConectado = isConnected();
    if (clienteConectado)
        return;

    Serial.println("Conexão WebSocket perdida.");

    connected = connectSK();
    if (connected)
    {
        resetTime();
    }
}

bool Socket::isConnected()
{
    return webSocket.available();
}
bool Socket::connectWIFI()
{
    Serial.println("Tentando conectar ao WiFi...");
    WiFi.begin(ssid, passwd);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\nFalha ao conectar ao WiFi!");
        return false;
    }

    Serial.println("\nConectado ao WiFi!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    return true;
}

bool Socket::connectSK()
{
    Serial.println("Tentando conectar ao WebSocket...");
    unsigned long startConnectTime = millis();
    int attempt = 0;

    while (!webSocket.connect(host) && millis() - startConnectTime < 100000)
    {
        attempt++;
        delay(1000);
        Serial.printf("Tentativa %d de conexão ao WebSocket falhou! Tentando novamente...\nHost: %s\n", attempt, host);
    }

    if (millis() - startConnectTime > 100000)
    {
        connected = false;
        Serial.println("Falha ao conectar no WebSocket após várias tentativas.");
        return false;
    }

    connected = true;
    resetTime();
    Serial.println("Conectado ao WebSocket!");
    return true;
}