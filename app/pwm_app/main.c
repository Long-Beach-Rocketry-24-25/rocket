
#include "FreeRTOS.h"
#include "task.h"

#include "pwm_app_bsp.h"

#include "gpio.h"
#include "st_pwm.h"

#include "debug_app.h"

/*-----------------------------------------------------------*/

Pwm pwm;
Gpio led_gpio;
Usart usart;
I2c i2c;

int main(void)
{

    BSP_Init(&pwm, &led_gpio);

    DebugAppCreate(&usart, &i2c, &led_gpio);

    /* Start the scheduler to start the tasks executing. */
    //vTaskStartScheduler();

    size_t x = 0;

    while (1){
        StPwmDuty(&pwm, x++);
        HAL_Delay(1);
    }

    return 0;
}
