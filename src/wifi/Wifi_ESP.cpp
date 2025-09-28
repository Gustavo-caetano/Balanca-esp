#include "Wifi_ESP.hpp"
#include <WiFi.h>
#include <vector>


void Wifi::init(std::vector<std::string> conexao, bool standalone)
{
    this->ssid = conexao[0];
    this->passwd = conexao[1];

    // if(!standalone) connect();
    connect();
}

bool Wifi::connect()
{
    Serial.println("Tentando conectar ao WiFi... nova versao");
    
    if(ssid.empty() || passwd.empty())
    {
        Serial.println("\nWifi ainda não configurado\ncrie uma configuração de conexão");
        return false;
    }

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
