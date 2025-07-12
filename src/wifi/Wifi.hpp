#ifndef WIFI_HPP
#define WIFI_HPP

#include <WiFi.h>
#include <vector>

class Wifi {
  private:
    std::string ssid;
    std::string passwd;

    bool connect();
  public:
    void init(std::vector<std::string> conexao, bool standalone);
};

#endif // WIFI_HPP 