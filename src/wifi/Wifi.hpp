#ifndef WIFI_HPP
#define WIFI_HPP

#include <WiFi.h>
#include <eprom/Eprom.hpp>

class Wifi {
  private:
    String ssid;
    String passwd;
    Eprom eprom;

  public:
    void init(String ssid, String passwd);
    bool connect();
    void selectWifi();
    void addWifi();
    void removeWifi();
    void menu();
    String lerDados(long time = 3000);
};

#endif // WIFI_HPP 