#include "OtaUpdate.hpp"

#include "./utils/versionutils/VersionUtils.hpp"

#include <Arduino.h>  
#include <ArduinoJson.h>

void OtaUpdate::iniciar(const char* firmwareUrl) {
    this->firmwareUrl = firmwareUrl;
}

FirmwareInfo OtaUpdate::getFirmwareInfo(const char* url) {
  FirmwareInfo info = {"", ""};

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      info.version = doc["version"] | "";
      info.firmware = doc["firmware"] | "";
    } else {
      Serial.print("Erro ao parsear JSON: ");
      Serial.println(error.c_str());
    }
  } else {
    Serial.printf("Falha HTTP, code: %d\n", httpCode);
  }

  http.end();
  return info;
}

std::string OtaUpdate::atualizarHTTP(std::string version) {
    Serial.println("Iniciando HTTP OTA... \nObtendo versão do servidor");


    std::string url = firmwareUrl + "/version";
    FirmwareInfo data = getFirmwareInfo(url.c_str());
    if(VersionUtils::isVersionGreaterOrEqual(version, data.version.c_str())) {
        Serial.println(("versao atual: " + version + " é a mais recente").c_str());
        return "";
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi não conectado!");
        return "";
    }

    Serial.printf("Heap livre: %u bytes\n", ESP.getFreeHeap());
    if (ESP.getFreeHeap() < 40000) {
        Serial.println("Heap insuficiente para OTA HTTP");
        return "";
    }

    HTTPClient http;
    if (!http.begin((firmwareUrl + "/firmware/" + data.firmware).c_str())) {
        Serial.println("Falha ao iniciar HTTP");
        return "";
    }

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("Erro HTTP: %d\n", httpCode);
        http.end();
        return "";
    }

    int contentLength = http.getSize();
    if (contentLength <= 0) {
        Serial.println("Aviso: sem Content-Length. Tentando mesmo assim...");
    }

    WiFiClient* client = http.getStreamPtr();
    if (!Update.begin(contentLength > 0 ? contentLength : UPDATE_SIZE_UNKNOWN)) {
        Serial.println("Espaço insuficiente para OTA.");
        http.end();
        return "";
    }

    size_t written = 0;
    uint8_t buffer[512];
    unsigned long lastProgress = millis();
    const unsigned long OTA_TIMEOUT = 15000; // 15 segundos sem progresso -> erro

    while ((contentLength <= 0) || (written < (size_t)contentLength)) {
        // timeout se ficar parado
        if (millis() - lastProgress > OTA_TIMEOUT) {
            Serial.println("Erro: Timeout durante OTA");
            Update.abort();
            http.end();
            return "";
        }

        size_t size = client->available();
        if (size) {
            size = (size > sizeof(buffer)) ? sizeof(buffer) : size;
            int c = client->readBytes(buffer, size);
            if (c <= 0) {
                Serial.println("Erro: leitura falhou ou conexão fechada");
                break;
            }

            size_t w = Update.write(buffer, c);
            if (w != (size_t)c) {
                Serial.println("Erro: falha ao gravar na flash");
                break;
            }

            written += w;
            lastProgress = millis(); // reset timeout
            if (contentLength > 0) {
                Serial.printf("Progresso: %d%%\r", (written * 100) / contentLength);
            } else {
                Serial.printf("Baixados: %u bytes\r", written);
            }
        }
        delay(1);

        // Se o servidor fechou a conexão e não temos Content-Length
        if (contentLength <= 0 && !client->connected() && !client->available()) {
            Serial.println("\nConexão fechada pelo servidor (fim do arquivo)");
            break;
        }
    }

    if (!Update.end() || !Update.isFinished()) {
        Serial.printf("Erro Update.end(): %s\n", Update.errorString());
        http.end();
        return "";
    }

    Serial.println("\nUpdate HTTP concluído. Reiniciando...");
    http.end();
    return data.version;

}
