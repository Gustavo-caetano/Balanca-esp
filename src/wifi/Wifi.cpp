#include "WiFi.hpp"
#include <WiFi.h>
#include <vector>

void Wifi::init(std::vector<String> conexao)
{
    this->ssid = conexao[0];
    this->passwd = conexao[1];

    connect();
}

bool Wifi::connect()
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
        Serial.println("Ssid: " + ssid +'\n' + "Senha: "+ passwd);
        return false;
    }

    Serial.println("\nConectado ao WiFi!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    return true;
}
