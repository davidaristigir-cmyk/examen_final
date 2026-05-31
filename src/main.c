#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)   // ← punto de entrada OBLIGATORIO en ESP-IDF
{
    while (1) {
        // tu código aquí
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}