#include "gpio_test_bsp.h"
#include <stdint.h>
#include "core_cm4.h"  // SysTick

// ===== LED and Button GPIO Definitions =====

//NEED TO ENABLE GPIO

static StGpioParams led_stgpio = {{0},
                                  GPIOA_BASE,
                                  5,
                                  {GPOUT, 0, 0, 0, 0}};  //NEEDS ENABLE
static StGpioParams button_stgpio = {{0},
                                     GPIOA_BASE,
                                     0,
                                     {GPIN, 0, 0, 0, 0}};  //NEEDS ENABLE

static Gpio led_gpio;
static DebounceButton_t button;

// ===== Timekeeping =====
static volatile uint32_t system_ms = 0;

void SysTick_Handler(void)
{
    system_ms++;
}

static uint32_t get_time_ms(void)
{
    return system_ms;
}

// ===== Debounce API =====
void Debounce_Init(DebounceButton_t* btn, StGpioParams* gpio_params,
                   uint32_t debounce_time_ms)
{
    StGpioInit(&btn->gpio, gpio_params);
    StGpioConfig(&btn->gpio);

    btn->debounce_time_ms = debounce_time_ms;
    btn->state = BUTTON_RELEASED;
    btn->raw_state = 0;
    btn->last_change_ms = 0;
}

void Debounce_Update(DebounceButton_t* btn, uint32_t now_ms)
{
    uint8_t raw = StGpioRead(&btn->gpio);

    if (raw != btn->raw_state)
    {
        btn->last_change_ms = now_ms;
        btn->raw_state = raw;
    }

    if ((now_ms - btn->last_change_ms) >= btn->debounce_time_ms)
    {
        if (raw && btn->state == BUTTON_RELEASED)
        {
            btn->state = BUTTON_PRESSED;
        }
        else if (!raw && btn->state == BUTTON_PRESSED)
        {
            btn->state = BUTTON_RELEASED;
        }
    }
}

uint8_t Debounce_IsPressed(DebounceButton_t* btn)
{
    return btn->state == BUTTON_PRESSED;
}

uint8_t Debounce_IsReleased(DebounceButton_t* btn)
{
    return btn->state == BUTTON_RELEASED;
}

DebounceState Debounce_GetState(DebounceButton_t* btn)
{
    return btn->state;
}

// System Clock Stub
void SystemInit(void)
{
}

//  Main Function
int main(void)
{
    // Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  //NEEDS ENABLE

    // Init LED and Button GPIOs
    StGpioInit(&led_gpio, &led_stgpio);
    StGpioConfig(&led_gpio);

    Debounce_Init(&button, &button_stgpio, 50);  // 50ms debounce

    // Configure SysTick for 1ms ticks
    SysTick_Config(SystemCoreClock / 1000);  //NEEDS ENABLE

    while (1)
    {
        Debounce_Update(&button, get_time_ms());

        if (Debounce_IsPressed(&button))
        {
            StGpioWrite(&led_gpio, 1);  // LED ON
        }

        if (Debounce_IsReleased(&button))
        {
            StGpioWrite(&led_gpio, 0);  // LED OFF
        }
    }
}
