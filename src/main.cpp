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

void menu(String opcao);
void alteracaoDeCalibracao();

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando...");
  balanca.iniciar();
  alteracaoDeCalibracao();

  wifi.init(
      "PROJETO1",
      "umdoistres");

  socket.iniciar("ws://10.0.0.12:15000");
  // socket.onMenssage(menu);
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

void menu(String opcao)
{
  switch (std::stoi(opcao.c_str()))
  {
  case 1:

    break;

  default:
    break;
  }
}

void alteracaoDeCalibracao()
{
  String calibracao;
  do
  {
    Serial.println("Calibracao informe o modelo:\nA{numero} para aumentar o numero\nD{numero} para diminuir o numero\nPara sair aperte o 0");
    Serial.setTimeout(700000);
    calibracao = Serial.readStringUntil('\n');
    if (calibracao[0] == 'A')
    {
      Serial.println("Aumentou");
      calibration_factor += std::stoi(calibracao.substring(1, calibracao.length()).c_str());
    }
    else if (calibracao[0] == 'B')
    {
      Serial.println("Diminuiu");
      calibration_factor -= std::stoi(calibracao.substring(1, calibracao.length()).c_str());
    }
    float peso = balanca.getPeso();
    Serial.println(peso);
  } while (calibracao != "0");
}