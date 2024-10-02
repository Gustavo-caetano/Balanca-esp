#include "Balanca.hpp"
#include <Arduino.h>

#define pinDT  26
#define pinSCK  27

Balanca::Balanca() : active(false) {}

void Balanca::iniciar() {
  scale.begin(pinDT, pinSCK);
  scale.set_scale();
  scale.tare();
  Serial.println("Balança zerada.");
}

float Balanca::getPeso() {
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

void Balanca::setScale(float calibration) {
  scale.set_scale(calibration);
};