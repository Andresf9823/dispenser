#ifndef _INPUTS_OUTPUTS_HPP
#define _INPUTS_OUTPUTS_HPP

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/freertos.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define BLINK_GPIO GPIO_NUM_2

class InputsOutputs {
private:
	const char *tag = "GPIO";
	TimerHandle_t blinkTimerHandler;
	uint32_t numberOfTimers = 0;
	static void blinkTimerCallback(TimerHandle_t pdTimer);

public:
	InputsOutputs();
	void InitBlink();
	void (*logString)(const char *TAG, const char *message);
	void (*logDword)(const char *TAG, uint32_t logNumber);
	void (*logFloat)(const char *TAG, double logFloating);
	~InputsOutputs();
};

#endif
