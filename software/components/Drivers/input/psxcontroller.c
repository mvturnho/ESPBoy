// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include <driver/adc.h>
#include "driver/gpio.h"
#include "soc/gpio_struct.h"
#include "psxcontroller.h"
#include "sdkconfig.h"
#include "pins.h"
// #include <hal_audio.h>

#define DELAY() asm("nop; nop; nop; nop;nop; nop; nop; nop;nop; nop; nop; nop;nop; nop; nop; nop;")

static void psxDone()
{
    DELAY();
}

int psxReadInput() {
	uint8_t BUTTON_UP = 1, BUTTON_DOWN = 1, BUTTON_LEFT = 1, BUTTON_RIGHT = 1, SOFT_RESET = 1, HARD_RESET = 1;
	uint8_t b1, b2;

	uint16_t joyX = adc1_get_raw(JOY_X_AXIS);
    uint16_t joyY = adc1_get_raw(JOY_Y_AXIS);
	// uint16_t joy_vol = adc1_get_raw(JOY_VOL);

	if(joyX > 3000)
	{
		BUTTON_LEFT = 0;
	}
	else if(joyX > 1000)
	{
		BUTTON_RIGHT = 0;
	}

	if(joyY > 3000)
	{
		BUTTON_UP = 0;
	}
	else if(joyY > 1000)
	{
		BUTTON_DOWN = 0;
	}

	// printf("JOY_X: %d - ", joyX);
	// printf("\tJOY_Y: %d - ", joyY);
	// // printf("\n");
	// printf("U:%d-", !BUTTON_UP);
	// printf("-D:%d", !BUTTON_DOWN);
	// printf("-R:%d", !BUTTON_RIGHT);
	// printf("-L:%d", !BUTTON_LEFT);
	// // printf("\n");
	// printf("-SELECT:%d", !BUTTON_SELECT);
	// printf("-START:%d", !BUTTON_START);
	// printf("-A:%d", !BUTTON_A);
	// printf("-B:%d\n", !BUTTON_B);

	SOFT_RESET = ((BUTTON_SELECT==0) && (BUTTON_START==0));
	b2 = HARD_RESET<<7 | (BUTTON_B<<6) | (BUTTON_A<<5) | SOFT_RESET<<4 | 1<<3 | 1<<2 | 1<<1 | 1<<0;
	b1 = (BUTTON_LEFT<<7) | (BUTTON_DOWN<<6) | (BUTTON_RIGHT<<5) | (BUTTON_UP<<4) | (BUTTON_START<<3) | 1<<2 | 1<<1 | (BUTTON_SELECT<<0);

	psxDone();
	return (b2<<8)|b1;
}

void psxcontrollerInit() {
	
	// audio_amplifier_init();
	// amplifier_set_off();
	// amplifier_set_on();

	adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(JOY_X_AXIS, ADC_ATTEN_11db);
	adc1_config_channel_atten(JOY_Y_AXIS, ADC_ATTEN_11db);
	adc1_config_channel_atten(JOY_VOL, ADC_ATTEN_11db);

	gpio_pad_select_gpio(JOY_SELECT);
	gpio_pad_select_gpio(JOY_START);
	gpio_pad_select_gpio(JOY_A);
	gpio_pad_select_gpio(JOY_B);
	
	gpio_set_direction(JOY_SELECT, GPIO_MODE_INPUT);
	gpio_set_direction(JOY_START, GPIO_MODE_INPUT);
	gpio_set_direction(JOY_A, GPIO_MODE_INPUT);
	gpio_set_direction(JOY_B, GPIO_MODE_INPUT);

	gpio_set_pull_mode(JOY_SELECT, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(JOY_START, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(JOY_A, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(JOY_B, GPIO_PULLUP_ONLY);

	// jack_sense_init();
}