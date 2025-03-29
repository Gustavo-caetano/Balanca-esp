#include "Socket.hpp"

Socket::Socket() : tempo(0), connected(false) {}

void Socket::iniciar(String server)
{
    this->host = server;
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

bool Socket::connectSK()
{
    Serial.println("Tentando conectar ao WebSocket...");
    unsigned long startConnectTime = millis();
    int attempt = 0;

    while (!webSocket.connect(host) && millis() - startConnectTime < 100000)
    {
        attempt++;
        delay(1000);
        Serial.println("Tentativa " + String(attempt) +" de conexão ao WebSocket falhou! Tentando novamente...\nHost:"+ host);
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

void Socket::onMenssage(void (*funcao)(String opcao))
{
    webSocket.onMessage([funcao](websockets::WebsocketsMessage mensage){
        funcao(mensage.data());
    });
}

void Socket::poll()
{
    webSocket.poll();
}