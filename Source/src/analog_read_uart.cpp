#include "analog_read_uart.h"

void AnalogRead_UART(void *pvParameters) 
{
    // Set ADC resolution to 12-bit (range: 0-4095)
    analogReadResolution(12);

    // Configure UART_NUM_0 parameters using ESP-IDF API
    uart_config_t uart_config = {
        .baud_rate = 115200,                  // Communication speed
        .data_bits = UART_DATA_8_BITS,        // 8 data bits per frame
        .parity = UART_PARITY_DISABLE,        // No parity bit
        .stop_bits = UART_STOP_BITS_1,        // 1 stop bit
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE // Disable hardware flow control
    };
    uart_param_config(UART_NUM, &uart_config);
    
    // Set UART pins (TX: 43, RX: 44)
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    // Install UART driver with a 256-byte TX buffer (RX buffer is 0 as we only send)
    uart_driver_install(UART_NUM, 256, 0, 0, NULL, 0);

    char send_buffer[50];
    
    while (1) 
    {
        // 1. Read the analog value from the configured sensor pin
        int adcValue = analogRead(SENSOR_PIN);
        
        // 2. Format the output string (append \r\n for terminal line break)
        int len = sprintf(send_buffer, "ADC Value from ESP32: %d\r\n", adcValue);
        
        // 3. Transmit the formatted string over UART to the CP2102 module
        uart_write_bytes(UART_NUM, send_buffer, len); 

        // 4. Print the exact same string to Native USB Serial for debugging
        Serial.printf("Transmitting: %s", send_buffer);

        // Suspend the task for 1000 milliseconds (1 second)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}