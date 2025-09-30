#ifndef OTAUPDATE_HPP
#define OTAUPDATE_HPP

#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <Update.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "Arduino.h"

struct FirmwareInfo {
  std::string version;
  std::string firmware;
};
class OtaUpdate {
private:
    std::string firmwareUrl;
    FirmwareInfo getFirmwareInfo(const char* url);
public:
    void iniciar(const char* firmwareUrl);
    std::string atualizarHTTP(std::string version);
};

#endif // OTAUPDATE_HPP
