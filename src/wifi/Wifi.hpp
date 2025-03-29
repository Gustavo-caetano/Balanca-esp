#ifndef WIFI_HPP
#define WIFI_HPP

#include <WiFi.h>
#include <vector>

class Wifi {
  private:
    String ssid;
    String passwd;

    bool connect();
  public:
    void init(std::vector<String> conexao);
};

#endif // WIFI_HPP 