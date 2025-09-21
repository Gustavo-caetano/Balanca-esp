#include "Balanca.hpp"
#include <Arduino.h>

const short pinDT = 26;
const short pinSCK = 27;

Balanca::Balanca() : active(false) {}

void Balanca::iniciar(float calibrationFactor)
{
  scale.begin(pinDT, pinSCK);

  this->calibrationFactor = calibrationFactor;
  tara();
}

float Balanca::getPeso()
{

  scale.set_scale(calibrationFactor);
  // float peso = scale.get_units() * 1000;
  float peso = random(10, 500);
  active = peso > 10;

  return peso;
}

bool Balanca::isActive()
{
  return active;
}

void Balanca::tara()
{
  scale.tare();
  Serial.println("Balança zerada.");
}

void Balanca::setScale(float calibration)
{
  calibrationFactor += calibration;
  Serial.println("Calibrado: " + String(calibrationFactor));
};

float Balanca::getScale()
{
  return calibrationFactor;
}
