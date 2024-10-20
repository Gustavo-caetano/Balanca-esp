#include "Balanca.hpp"
#include <Arduino.h>

#define pinDT 26
#define pinSCK 27

Balanca::Balanca() : active(false), calibration_factor(277560.00) {}

void Balanca::iniciar()
{
  scale.begin(pinDT, pinSCK);
  tara();
}

float Balanca::getPeso()
{

  scale.set_scale(calibration_factor);
  float peso = scale.get_units() * 1000;
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

  calibration_factor += calibration;
};