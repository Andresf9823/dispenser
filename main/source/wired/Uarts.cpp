#include <wire/Uarts.hpp>

Uarts::Uarts() // @suppress("Class members should be properly initialized")
{
}

void Uarts::scanUart0Rx(void *pvParameters)
{
	uint8_t *buffer;
	buffer = (uint8_t *)malloc(UART_RX_BUFFER_SIZE);
	memset(buffer, 0, UART_RX_BUFFER_SIZE);
	while (1)
	{
		uint8_t bytesRead = uart_read_bytes(UART_NUM_0, buffer, UART_RX_BUFFER_SIZE, pdMS_TO_TICKS(50));
		if (bytesRead > 0)
		{
		}
	}
	free(buffer);
}

void Uarts::scanUart1Rx(void *pvParameters)
{

	uint8_t *buffer;
	buffer = (uint8_t *)malloc(UART_RX_BUFFER_SIZE);
	memset(buffer, 0, UART_RX_BUFFER_SIZE);
	while (1)
	{
		uint8_t bytesRead = uart_read_bytes(UART_NUM_1, buffer, sizeof(buffer), pdMS_TO_TICKS(50));
		if (bytesRead > 0)
		{
		}
	}
	free(buffer);
}

void Uarts::scanUart2Rx(void *pvParameters)
{
	uint8_t *buffer;
	buffer = (uint8_t *)malloc(UART_RX_BUFFER_SIZE);
	memset(buffer, 0, UART_RX_BUFFER_SIZE);
	while (1)
	{
		uint8_t bytesRead = uart_read_bytes(UART_NUM_2, buffer, sizeof(buffer), pdMS_TO_TICKS(50));
		if (bytesRead > 0)
		{
		}
	}
	free(buffer);
}

bool Uarts::uartInitializer(uint8_t uartNumber)
{
	if (uartNumber < 3)
	{
		const uart_config_t uartDefaultConfig = {.baud_rate = 9600, .data_bits = UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE, .stop_bits = UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, .rx_flow_ctrl_thresh = (uint8_t)UART_RX_BUFFER_SIZE, .source_clk = UART_SCLK_DEFAULT};

		switch (uartNumber)
		{
		case 0:
			uart_param_config(UART_NUM_0, &uartDefaultConfig);
			uart_set_pin(UART_NUM_0, TX0_PIN, RX0_PIN, UART_PIN_NO_CHANGE,
						 UART_PIN_NO_CHANGE);
			uart_driver_install(UART_NUM_0,
								configMINIMAL_STACK_SIZE, 0, 0,
								NULL, 0);
			xTaskCreate(scanUart0Rx, "UART0", configMINIMAL_STACK_SIZE,
						NULL, 5, NULL);
			break;
		case 1:
			uart_param_config(UART_NUM_1, &uartDefaultConfig);
			uart_set_pin(UART_NUM_1, TX1_PIN, RX2_PIN, UART_PIN_NO_CHANGE,
						 UART_PIN_NO_CHANGE);
			uart_driver_install(UART_NUM_1,
								configMINIMAL_STACK_SIZE, 0, 0,
								NULL, 0);
			xTaskCreate(scanUart1Rx, "UART1", configMINIMAL_STACK_SIZE,
						NULL, 5, NULL);
			break;
		case 2:
			uart_param_config(UART_NUM_2, &uartDefaultConfig);
			uart_set_pin(UART_NUM_2, TX2_PIN, RX2_PIN, UART_PIN_NO_CHANGE,
						 UART_PIN_NO_CHANGE);
			uart_driver_install(UART_NUM_2,
								configMINIMAL_STACK_SIZE, 0, 0,
								NULL, 0);
			xTaskCreate(scanUart2Rx, "UART2", configMINIMAL_STACK_SIZE,
						NULL, 5, NULL);
			break;
		}
		return true;
	}
	this->logString(tag, "HW for Uart interface don't exist:");
	this->logDword(tag, uartNumber);
	return false;
}
