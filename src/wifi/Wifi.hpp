#ifndef WIFI_HPP
#define WIFI_HPP

#include <WiFi.h>

class Wifi {
  private:
    String ssid;
    String passwd;

  public:
    void init(String ssid, String passwd);
    bool connect();
};

#endif // WIFI_HPP 