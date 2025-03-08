
#include "FreeRTOS.h"
#include "main_loop.h"
#include "task.h"

#include "pwm_app_bsp.h"

#include "gpio.h"
#include "st_pwm.h"

/*-----------------------------------------------------------*/

Pwm pwm;
Gpio led_gpio;
static StGpioParams led_stgpio = {{0}, GPIOA_BASE, 5, {ALT_FUNC, 0, 0, 0, 0x1}};

void pwm_test(void)
{
    static size_t x = 0;

    pwm.setDuty(&pwm, x);

    x = (x < 100) ? x + 1 : 0;
}

int main(void)
{

    BSP_Init(&pwm, &led_gpio, &led_stgpio);

    StPwmEnable(&pwm, true);

    create_main_loop(pwm_test, 10);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    return 0;
}
