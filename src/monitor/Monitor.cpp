#include "Monitor.hpp"
#include <Arduino.h>
#include "esp_system.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void Monitor::monitor(void *pvParameters) {
    while (1) {

        const char* chipModel = ESP.getChipModel(); 

        // Contagem de Tarefas
        uint32_t taskCount = uxTaskGetNumberOfTasks();

        // Voltagem (Vdd do ESP32)
        int adcRead = adc1_get_raw(ADC1_CHANNEL_0); // Leitura do ADC para Vdd
        float voltage = adcRead * (3.3 / 4095.0);   // Converter a leitura ADC para tensão (valor de 3.3V)

        // Utilização de RAM (Memória)
        uint32_t freeRam = ESP.getFreeHeap();
        uint32_t totalRam = ESP.getHeapSize();
        float ramUsage = ((totalRam - freeRam) * 100.0) / totalRam;

        // Frequência dos núcleos de CPU
        uint32_t cpuFreqCore0 = getCpuFrequencyMhz();
        uint32_t cpuFreqCore1 = getCpuFrequencyMhz(); // Isso vai retornar a mesma frequência, já que o ESP32 geralmente tem a mesma frequência em ambos os núcleos

        // Contagem de ciclo de CPU para ambos os núcleos
        uint32_t cpuUsageCore0 = 0;
        uint32_t cpuUsageCore1 = 0;
        
        // Obter o ID do núcleo em que a tarefa está sendo executada
        uint32_t coreId = xPortGetCoreID();
        if (coreId == 0) {
            cpuUsageCore0++;
        } else if (coreId == 1) {
            cpuUsageCore1++;
        }

        Serial.printf("\n=== Monitoramento do ESP32 ===\n");
        Serial.printf("Modelo do Chip: %s\n", chipModel);
        Serial.printf("Frequência da CPU Core 0: %d MHz\n", cpuFreqCore0);
        Serial.printf("Frequência da CPU Core 1: %d MHz\n", cpuFreqCore1);
        Serial.printf("CPU Tempo de Tarefa: %lu ticks\n", xTaskGetTickCount());
        Serial.printf("Número de Tarefas: %d\n", taskCount);
        Serial.printf("Voltagem Vdd: %.2f V\n", voltage);
        Serial.printf("Uso de RAM: %.2f%% (Livre: %d bytes)\n", ramUsage, freeRam);
        Serial.printf("Contagem de CPU Core 0: %d\n", cpuUsageCore0);
        Serial.printf("Contagem de CPU Core 1: %d\n", cpuUsageCore1);

        vTaskDelay(pdMS_TO_TICKS(2000));  // Atualiza a cada 2 segundos
    }
}

void Monitor::monitorTask() {
    xTaskCreate(
        Monitor::monitor,
        "MonitorEsp",
        2048,
        NULL,
        1,
        NULL
    );
}
