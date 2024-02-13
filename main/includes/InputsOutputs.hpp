#ifndef _INPUTS_OUTPUTS_HPP
#define _INPUTS_OUTPUTS_HPP

#include <GlobalDefines.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <driver/gpio.h>

#define BLINK_GPIO GPIO_NUM_2

using namespace std;

class InputsOutputs
{
private:
	static constexpr string tag = "GPIO";
	TimerHandle_t blinkTimerHandler;
	uint32_t numberOfTimers = 0;
	static void blinkTimerCallback(TimerHandle_t pdTimer);

public:
	InputsOutputs();
	void InitBlink();
	void (*logString)(string TAG, string message);
	void (*logDword)(string TAG, int32_t logNumber);
	void (*logFloat)(string TAG, double logFloating);
	~InputsOutputs();
};

#endif
