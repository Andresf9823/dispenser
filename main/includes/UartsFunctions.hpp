#ifndef _UARTS_FUNCTIONS_HPP_
#define _UARTS_FUNCTIONS_HPP_

#include <GlobalDefines.hpp>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include <freertos/timers.h>
#include <driver/uart.h>
#include <driver/gpio.h>

using namespace std;

#define TX0_PIN GPIO_NUM_1
#define RX0_PIN GPIO_NUM_3

#define TX1_PIN GPIO_NUM_14
#define RX1_PIN GPIO_NUM_12

#define TX2_PIN GPIO_NUM_17
#define RX2_PIN GPIO_NUM_16

#define UART_RX_BUFFER_SIZE 512

static uint8_t *uart0Buffer;
static uint8_t *uart1Buffer;
static uint8_t *uart2Buffer;

class Uarts
{
public:
	Uarts();
	static constexpr string tag = "UART";
	bool UartInitializer(uint8_t uartNumber);
	void (*logString)(string TAG, string message);
	void (*logDword)(string TAG, int32_t logNumber);
	void (*logFloat)(string TAG, double logFloating);
	~Uarts();

private:
	static void ScanUart0Rx(void *pvParameters);
	static void ScanUart1Rx(void *pvParameters);
	static void ScanUart2Rx(void *pvParameters);
};

#endif
