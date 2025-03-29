#ifndef BALANCA_HPP
#define BALANCA_HPP

#include <HX711.h>

class Balanca {
  private:
    HX711 scale;
    bool active;
    float calibrationFactor;

  public:
    Balanca();
    void iniciar(float calibrationFactor);
    float getPeso();
    bool isActive();
    void tara();
    void setScale(float calibration);
    float getScale();
};

#endif // BALANCA_HPP
