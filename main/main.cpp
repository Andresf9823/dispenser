#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <esp_log.h>
#include <JsonFormatter.hpp>
#include <InputsOutputs.hpp>
#include <TaskManager.hpp>
#include <TcpService.hpp>
#include <UartsFunctions.hpp>

using namespace std;

/*Memory size*/
#define KB (1024UL)
#define MB (KB)(KB)

static const char *tag = "Main";

TaskManager *Task;
Uarts *Uart;
JsonFormatter *Json;
InputsOutputs *Gpio;
TcpService *Tcp;

void logString(const char *TAG, const char *message)
{
	ESP_LOGI(TAG, "%s", (unsigned char *)message);
}

void logDword(const char *TAG, uint32_t logNumber)
{
	char charNumber[KB];
	sprintf(charNumber, "%ld", logNumber);
	logString(TAG, charNumber);
}

void logFloat(const char *TAG, double logFloating)
{
	char charFloating[KB];
	sprintf(charFloating, "%f.2", logFloating);
	logString(TAG, (char*) charFloating);
}

void CheckSomethingInUarts()
{
	if (uart0Buffer != NULL) {
		logString(tag, "This incoming from uart0: ");
		logString(tag, (const char*) uart0Buffer);
	}
	if (uart1Buffer != NULL) {
		logString(tag, "This incoming from uart1: ");
		logString(tag, (const char*) uart1Buffer);
	}
	if (uart2Buffer != NULL) {
		logString(tag, "This incoming from uart2: ");
		logString(tag, (const char*) uart2Buffer);
	}
}

void initObjects()
{
	Uart = new Uarts();
	Uart->logString = logString;
	Uart->logDword = logDword;
	Uart->logFloat = logFloat;
	Uart->UartInitializer(2);

	Json = new JsonFormatter();
	Json->logString = logString;
	Json->logDword = logDword;
	Json->logFloat = logFloat;

	Task = new TaskManager();
	Task->logString = logString;
	Task->logDword = logDword;
	Task->logFloat = logFloat;

	Gpio = new InputsOutputs();
	Gpio->logString = logString;
	Gpio->logDword = logDword;
	Gpio->logFloat = logFloat;
	Gpio->InitBlink();

	Tcp = new TcpService();
	Tcp->logString = logString;
	Tcp->logDword = logDword;
	Tcp->logFloat = logFloat;
	Tcp->InitWifiService(WiFiMode::ApStation);
}

extern "C" int app_main(void)
{
	logString(tag, "Go project!");
	initObjects();
	while (1) {
		CheckSomethingInUarts();
	}
	return 0;
}
