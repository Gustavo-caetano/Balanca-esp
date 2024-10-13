#include "WiFi.hpp"
#include <WiFi.h>
#include <vector>

void Wifi::init(String ssid, String passwd)
{
    this->ssid = ssid;
    this->passwd = passwd;

    // menu();
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
        return false;
    }

    Serial.println("\nConectado ao WiFi!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    return true;
}

void Wifi::selectWifi()
{
    std::vector<String> redes = eprom.readWiFiList();
    int selecao;

    do
    {
        Serial.println("Selecione a rede, escolha o numero de 1 a o ultimo elemento");
        for (int i = 0; i < redes.size(); i++)
        {
            Serial.print((i + 1) + "- ");
            Serial.println(redes[i]);
        }
        selecao = lerDados().toInt();
    } while (selecao < 0 || selecao > redes.size());

    String wifi = redes[selecao - 1];

    WiFi.begin(wifi.substring(0, wifi.indexOf(':') - 1), wifi.substring(':') + 1, wifi.length());
}

void Wifi::addWifi()
{
    String ssid;
    String passwd;

    Serial.println("Informe o nome do wifi");
    ssid = lerDados(2000000);
    Serial.println("Informe a senha do wifi");
    passwd = lerDados(2000000);

    eprom.addWiFi(ssid, passwd);
}

void Wifi::removeWifi()
{
    std::vector<String> redes = eprom.readWiFiList();
    int selecao;

    do
    {
        Serial.println("Selecione a rede, escolha o numero de 1 a o ultimo elemento");
        for (int i = 0; i < redes.size(); i++)
        {
            Serial.print((i + 1) + "- ");
            Serial.println(redes[i]);
        }
        selecao = lerDados().toInt();
    } while (selecao < 0 || selecao > redes.size());

    eprom.removeWiFi(selecao - 1);
}

void Wifi::menu()
{
    int selecao;
    do
    {
        Serial.println("Selecione uma das opções\n1-Selecionar a rede\n2-Adicionar uma rede\n3-Apagar uma rede");
        selecao = lerDados().toInt();

        switch (selecao)
        {
        case 1:
            selectWifi();
            break;
        case 2:
            addWifi();
            break;
        case 3:
            removeWifi();
            break;
        default:
            Serial.println("Opção invalida");
            selecao = 5;
        }
    } while (selecao > 2);
}

String Wifi::lerDados(long time)
{
    Serial.setTimeout(time);
    return Serial.readStringUntil('\n');
}