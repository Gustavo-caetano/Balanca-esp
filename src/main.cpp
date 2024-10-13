#include "socket/Socket.hpp"
#include "balanca/Balanca.hpp"
#include "wifi/Wifi.hpp"

Socket socket;
Balanca balanca;
Wifi wifi;

bool active = true;

float calibration_factor = 277560.00;

float count = 0;
bool crescente = true;

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando...");

  wifi.init(
    "PROJETO1",
    "umdoistres"
  );

  socket.iniciar(
      "ws://10.0.0.16:15000");
  balanca.iniciar();
}

void loop()
{
  balanca.setScale(calibration_factor);
  socket.checkConnection();
  if (socket.isConnected())
  {
    float peso = balanca.getPeso();
    socket.sendData(peso, balanca.isActive());
  }
  delay(20);
}