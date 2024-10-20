#ifndef BALANCA_HPP
#define BALANCA_HPP

#include <HX711.h>

class Balanca {
  private:
    HX711 scale;
    bool active;
    float calibration_factor;

  public:
    Balanca();
    void iniciar();
    float getPeso();
    bool isActive();
    void tara();
    void setScale(float calibration);
};

#endif // BALANCA_HPP
