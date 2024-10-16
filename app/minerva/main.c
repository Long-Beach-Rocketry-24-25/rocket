
#include "FreeRTOS.h"
#include "task.h"

#include "bsp.h"

#include "i2c.h"
#include "usart.h"
#include "usart_cli.h"
#include "gpio.h"

#include "tmp102.h"
#include "ads111x.h"

#include "blink.h"
#include "read_tmp102.h"

#include "gnc.h"

/*-----------------------------------------------------------*/

void read_adc(int argc, char* argv[]);

Usart usart;
I2c temp_i2c;
I2c an1_i2c;
I2c an2_i2c;
CanBus can;
Gpio led_gpio;

volatile bool can_data_rdy = false;

Tmp102 tmp;
#define NUM_ADC 4
#define MAX_ADC_CHANNELS 4
Ads111x adc[NUM_ADC] = {};
uint8_t channels[NUM_ADC][MAX_ADC_CHANNELS] = {
    {ADS_MUX_SING_P0, ADS_MUX_SING_P1, ADS_MUX_SING_P2, ADS_MUX_SING_P3},
    {ADS_MUX_SING_P0, ADS_MUX_SING_P1, ADS_MUX_SING_P2, ADS_MUX_SING_P3},
    {ADS_MUX_SING_P0, ADS_MUX_SING_P1, ADS_MUX_SING_P2, ADS_MUX_SING_P3},
    {ADS_MUX_SING_P0, ADS_MUX_SING_P1, ADS_MUX_SING_P2, ADS_MUX_SING_P3}
};

volatile int16_t ad_readings[NUM_ADC][MAX_ADC_CHANNELS] = {0};

uint8_t cdat[8] = {1, 2, 3, 4, 5, 6, 7, 8};
uint8_t rdat[8] = { 0 };

int main(void)
{

    BSP_Init(&usart, &temp_i2c, &an1_i2c, &an2_i2c, &can, &led_gpio);
    led_gpio.set(&led_gpio, true);

    can.send(&can, cdat, 8);
    while (!can_data_rdy);

    init_blink(&led_gpio);
    init_read_tmp102(&tmp);

    Command commands[3] = { 
        {"Blink", blink, "Blinks LED."},
        {"Temp", read_tmp102, "Reads temperature."},
        {"Adc", read_adc, "Reads all ADC channels."}
    };
    create_cli_task(&usart, commands, 3);
    cli_write("USING HW: STM32L452");

    Tmp102_Init(&tmp, &temp_i2c, TMP102_ADDR_GND);

    Ads111x_Init(&adc[0], &an1_i2c, ADS111X_ADDR_GND);
    Ads111x_Config(&adc[0], ADS_PGA_6_144V, ADS_CONFIG_MODE, ADS_DR_475_SPS);
    Ads111x_Init(&adc[1], &an1_i2c, ADS111X_ADDR_VDD);
    Ads111x_Config(&adc[1], ADS_PGA_6_144V, ADS_CONFIG_MODE, ADS_DR_475_SPS);
    Ads111x_Init(&adc[2], &an2_i2c, ADS111X_ADDR_VDD);
    Ads111x_Config(&adc[2], ADS_PGA_6_144V, ADS_CONFIG_MODE, ADS_DR_475_SPS);
    Ads111x_Init(&adc[3], &an2_i2c, ADS111X_ADDR_GND);
    Ads111x_Config(&adc[3], ADS_PGA_6_144V, ADS_CONFIG_MODE, ADS_DR_475_SPS);
    

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    for( ; ; )
    {
    }

    return 0;
}

void read_adc(int argc, char* argv[])
{
    cli_write("Reading ADCs, CH x ADC:");
    volatile TickType_t tick_s;
    volatile TickType_t tick_e;
    tick_s = xTaskGetTickCount();

    for (size_t ch = 0; ch < MAX_ADC_CHANNELS; ++ch)
    {
        for (size_t ad = 0; ad < NUM_ADC; ++ad)
        {
            adc[ad].start_sample(&adc[ad], channels[ad][ch]);
        }

        // 1-2 ms delay was good for the legit TI chips, 11-12 for amazon ones.
        vTaskDelay(12);

        for (size_t ad = 0; ad < NUM_ADC; ++ad)
        {
            ad_readings[ad][ch] = adc[ad].read_sample(&adc[ad]);
        }

        cli_write("[%d %d %d %d]", ad_readings[0][ch],
                ad_readings[1][ch], ad_readings[2][ch], ad_readings[3][ch]);
    }
    tick_e = xTaskGetTickCount();
    cli_write("Time: %dms", tick_e - tick_s);

}

void CAN1_RX0_IRQHandler(void)
{
	can_data_rdy = true;
    can.recv(&can, rdat, 8);
}