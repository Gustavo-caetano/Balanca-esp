#ifndef BALANCA_HPP
#define BALANCA_HPP

#include <HX711.h>

class Balanca {
  private:
    HX711 scale;
    bool active;

  public:
    Balanca();
    void iniciar();
    float getPeso();
    bool isActive();
    void setScale(float calibration);
};

#endif // BALANCA_HPP
