
#include "FreeRTOS.h"
#include "main_loop.h"
#include "task.h"

#include "pwm_app_bsp.h"

#include "gpio.h"
#include "st_pwm.h"

/*-----------------------------------------------------------*/

Pwm pwm;
Gpio led_gpio;

void pwm_test(void)
{

    size_t x = 0;
    while (x < 100)
    {
        pwm.setDuty(&pwm, x);
        x++;
    }
}

int main(void)
{

    BSP_Init(&pwm, &led_gpio);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    create_main_loop(pwm_test, 1000);

    return 0;
}
