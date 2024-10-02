#include "Socket.hpp"
#include "Balanca.hpp"

Socket socket;
Balanca balanca;

bool active = true;

float calibration_factor = 277560.00;

float count = 0;
bool crescente = true;

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando...");
  socket.iniciar(
      "SEC. DE OBRAS",
      "flamengo",
      "ws://192.168.0.137:15000");
  balanca.iniciar();
}

void loop()
{
  balanca.setScale(calibration_factor);
  socket.checkConnection();
  if (socket.isConnected())
  {
    float peso = balanca.getPeso();
    // float peso = count;
    socket.sendData(peso, balanca.isActive());
    // if (count > 5000)
    // {
    //   active = false;
    //   crescente = false;
    // }
    // else if (count == 0)
    // {
    //   active = true;
    //   crescente = true;
    // }
    // crescente ? count += 100 : count -= 100;
  }
  delay(20);
}