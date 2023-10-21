#include <UartsFunctions.hpp>

Uarts::Uarts() // @suppress("Class members should be properly initialized")
{
}

void Uarts::ScanUart0Rx(void *pvParameters)
{
	uart0Buffer = (uint8_t*) malloc(512);
	memset(uart0Buffer, 0, 512);
	while (1) {
		uint8_t bytesRead = uart_read_bytes(UART_NUM_0, uart0Buffer,
		      sizeof(uart0Buffer), pdMS_TO_TICKS(50));
		if (bytesRead > 0) {
		}
	}
	free(uart0Buffer);
}

void Uarts::ScanUart1Rx(void *pvParameters)
{
	uart1Buffer = (uint8_t*) malloc(512);
	memset(uart1Buffer, 0, 512);
	while (1) {
		uint8_t bytesRead = uart_read_bytes(UART_NUM_1, uart1Buffer,
		      sizeof(uart1Buffer), pdMS_TO_TICKS(50));
		if (bytesRead > 0) {
		}
	}
	free(uart1Buffer);
}

void Uarts::ScanUart2Rx(void *pvParameters)
{
	uart2Buffer = (uint8_t*) malloc(512);
	memset(uart2Buffer, 0, 512);
	while (1) {
		uint8_t bytesRead = uart_read_bytes(UART_NUM_2, uart2Buffer,
		      sizeof(uart2Buffer), pdMS_TO_TICKS(50));
		if (bytesRead > 0) {
		}
	}
	free(uart2Buffer);
}

bool Uarts::UartInitializer(uint8_t uartNumber)
{
	if (uartNumber < 3) {

		const uart_config_t uartDefaultConfig = { .baud_rate = 9600, .data_bits = UART_DATA_8_BITS, .parity = UART_PARITY_DISABLE, .stop_bits = UART_STOP_BITS_1, .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, .rx_flow_ctrl_thresh = (uint8_t) 512, .source_clk = UART_SCLK_DEFAULT };

		switch (uartNumber)
		{
		case 0:
			uart_param_config(UART_NUM_0, &uartDefaultConfig);
			uart_set_pin(UART_NUM_0, TX0_PIN, RX0_PIN, UART_PIN_NO_CHANGE,
			UART_PIN_NO_CHANGE);
			uart_driver_install(UART_NUM_0,
			configMINIMAL_STACK_SIZE, 0, 0,
			NULL, 0);
			xTaskCreate(ScanUart0Rx, "UART0", configMINIMAL_STACK_SIZE,
			NULL, 5, NULL);
			break;
		case 1:
			uart_param_config(UART_NUM_1, &uartDefaultConfig);
			uart_set_pin(UART_NUM_1, TX1_PIN, RX2_PIN, UART_PIN_NO_CHANGE,
			UART_PIN_NO_CHANGE);
			uart_driver_install(UART_NUM_1,
			configMINIMAL_STACK_SIZE, 0, 0,
			NULL, 0);
			xTaskCreate(ScanUart1Rx, "UART1", configMINIMAL_STACK_SIZE,
			NULL, 5, NULL);
			break;
		case 2:
			uart_param_config(UART_NUM_2, &uartDefaultConfig);
			uart_set_pin(UART_NUM_2, TX2_PIN, RX2_PIN, UART_PIN_NO_CHANGE,
			UART_PIN_NO_CHANGE);
			uart_driver_install(UART_NUM_2,
			configMINIMAL_STACK_SIZE, 0, 0,
			NULL, 0);
			xTaskCreate(ScanUart2Rx, "UART2", configMINIMAL_STACK_SIZE,
			NULL, 5, NULL);
			break;
		}
		return true;
	}
	return false;
}
