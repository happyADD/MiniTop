#include "FreeRTOS.h"
#include "task.h"

int count = 0;
double sec = 0;

void CountTask(void const * argument)
{

    while(1)
    {
        count++;
        vTaskDelay(1);
        if(count == 1000)
        {
            sec +=0.1;
            count = 0;
        }
    }
}
