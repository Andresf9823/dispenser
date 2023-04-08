#include "InputsOutputs.hpp"

InputsOutputs::InputsOutputs(/* args */)
{
}

void InputsOutputs::InitBlink()
{

    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    numberOfTimers++;
    blinkTimerHandler = xTimerCreate("Blink board",
                                     pdMS_TO_TICKS(1000),
                                     pdTRUE,
                                     (void *)&numberOfTimers,
                                     this->blinkTimerCallback);

    if (blinkTimerHandler == NULL || xTimerStart(blinkTimerHandler, 0) != pdPASS)
    {
        numberOfTimers--;
        this->logString(this->tag, "Timer blink cannot be opened");
        return;
    }
}

void InputsOutputs::blinkTimerCallback(TimerHandle_t pdTimer)
{
    static uint32_t blinkState = 0;
    blinkState = gpio_get_level(BLINK_GPIO) == blinkState ? 1 : 0;
    gpio_set_level(BLINK_GPIO, blinkState);
}

InputsOutputs::~InputsOutputs()
{
}