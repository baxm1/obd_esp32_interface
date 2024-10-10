

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "driver/gpio.h" // want to do this without this header file at some point

#define STACKDEPTH_BYTES 2*1024

#define K_LINE_PIN 5 // WARNING: 5 is a strapping pin. don't use it

TaskHandle_t obdTask = NULL;    // Task handler

void transmitByte(char byte_to_transmit){
    char *taskName = pcTaskGetName(NULL); // get the name of this task
    char i, bit_to_transmit;

    for(i=0; i<8; i=i+1){
        bit_to_transmit = (byte_to_transmit >> i) & 0x01;
        ESP_LOGI(taskName, "%d", bit_to_transmit);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void obdtask(void *param){
    char *taskName = pcTaskGetName(NULL); // get the name of this task

    // configure GPIO pins
    gpio_reset_pin(K_LINE_PIN);
    gpio_set_direction(K_LINE_PIN, GPIO_MODE_OUTPUT);

    for(;;){
        ESP_LOGI(taskName, "obdtask");

        gpio_set_level(K_LINE_PIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(K_LINE_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        transmitByte(0x33);            

        }
    return;
}

void app_main(void)
{   

    xTaskCreatePinnedToCore(
        &obdtask,               //Function to implement the task 
        "obdtask",              //Name of the task
        STACKDEPTH_BYTES,       //Stack size in words 
        NULL,                   //Task input parameter 
        configMAX_PRIORITIES-3, //Priority of the task 
        &obdTask,               //Task handle.
        tskNO_AFFINITY);        //Core where the task should run 

}
