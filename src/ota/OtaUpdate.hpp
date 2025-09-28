#ifndef OTAUPDATE_HPP
#define OTAUPDATE_HPP

#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <Update.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include "Arduino.h"

class OtaUpdate {
private:
    String firmwareUrl;

public:
    void iniciar(const char* firmwareUrl);
    bool atualizarHTTP();
    void atualizarHTTPAsync();
};

#endif // OTAUPDATE_HPP
