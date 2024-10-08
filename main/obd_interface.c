

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"

#define STACKDEPTH_BYTES 2*1024


TaskHandle_t obdTask = NULL;    // Task handler
static const char* TAG = "obdModule";

void obdtask(void *param){
    for(;;){
        ESP_LOGI(TAG, "obdtask");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    return;
}

void app_main(void)
{   

    xTaskCreatePinnedToCore(
        &obdtask,                //Function to implement the task 
        "obdtask",              //Name of the task
        STACKDEPTH_BYTES,       //Stack size in words 
        NULL,                   //Task input parameter 
        configMAX_PRIORITIES-3, //Priority of the task 
        &obdTask,                   //Task handle.
        tskNO_AFFINITY);        //Core where the task should run 


}
