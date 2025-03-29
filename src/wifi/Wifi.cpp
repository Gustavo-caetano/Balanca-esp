#include "WiFi.hpp"
#include <WiFi.h>
#include <vector>

void Wifi::init(std::vector<std::string> conexao)
{
    this->ssid = conexao[0];
    this->passwd = conexao[1];

    connect();
}

bool Wifi::connect()
{
    Serial.println("Tentando conectar ao WiFi...");
    WiFi.begin(ssid.c_str(), passwd.c_str());

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
    {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("\nFalha ao conectar ao WiFi!");
        Serial.println("Ssid: " + String(ssid.c_str()) +'\n' + "Senha: "+ String(passwd.c_str()));
        return false;
    }

    Serial.println("\nConectado ao WiFi!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    return true;
}
