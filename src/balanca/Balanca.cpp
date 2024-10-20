#include "Balanca.hpp"
#include <Arduino.h>

#define pinDT  26
#define pinSCK  27

Balanca::Balanca() : active(false), calibration_factor(277560.00) {}

void Balanca::iniciar() {
  scale.begin(pinDT, pinSCK);
  scale.tare();
  Serial.println("Balança zerada.");
}

float Balanca::getPeso() {
  scale.set_scale(calibration_factor);
  
  float peso = scale.get_units() * 1000;
  if (peso > 10) {
    active = true;
  } else if (peso < 10) {
    active = false;
  }
  return peso;
}

bool Balanca::isActive() {
  return active;
}

void Balanca::tara()
{
  scale.tare();
}

void Balanca::setScale(float calibration) {

  calibration_factor += calibration;
};