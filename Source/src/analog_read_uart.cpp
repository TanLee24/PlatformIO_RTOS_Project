#include "analog_read_uart.h"

// void AnalogRead_UART(void *pvParameters) 
// {
//     // Cấu hình chân ADC
//     analogReadResolution(12); // Độ phân giải 12-bit (0-4095)

//     // Cấu hình UART (ESP-IDF API)
//     uart_config_t uart_config = {
//         .baud_rate = 115200,
//         .data_bits = UART_DATA_8_BITS,
//         .parity = UART_PARITY_DISABLE,
//         .stop_bits = UART_STOP_BITS_1,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
//     };
//     uart_param_config(UART_NUM, &uart_config);
//     uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
//     uart_driver_install(UART_NUM, 256, 0, 0, NULL, 0);

//     char buffer[50];
    
//     for (;;) 
//     {
//         int adcValue = analogRead(SENSOR_PIN);
        
//         // Định dạng chuỗi và gửi qua UART
//         // int len = sprintf(buffer, "Analog Value: %d\n", adcValue);
//         // uart_write_bytes(UART_NUM, buffer, len); // Sử dụng uart_write_bytes
//         Serial.printf("Test Analog: %d\n", adcValue);
        
//         vTaskDelay(pdMS_TO_TICKS(1000)); // Chờ 1 giây
//     }
// }

void AnalogRead_UART(void *pvParameters) 
{
    // Set ADC resolution to 12-bit (0-4095)
    analogReadResolution(12);

    // 1. Configure UART parameters
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    // Install UART driver with TX and RX buffers (256 bytes each)
    uart_driver_install(UART_NUM, 256, 256, 0, NULL, 0);

    char send_buffer[50];
    uint8_t receive_buffer[50];
    
    while (1) 
    {
        // --- PART 1: READ ADC AND SEND VIA UART API ---
        int adcValue = analogRead(SENSOR_PIN);
        int len = sprintf(send_buffer, "ADC Value: %d", adcValue);
        
        // Transmit data out through the TX pin
        uart_write_bytes(UART_NUM, send_buffer, len); 

        // Wait 50ms for the signal to propagate to the RX pin
        vTaskDelay(pdMS_TO_TICKS(50)); 

        // --- PART 2: RECEIVE VIA UART API AND PRINT TO COMPUTER ---
        // Read the incoming data from the RX pin
        int rxBytes = uart_read_bytes(UART_NUM, receive_buffer, sizeof(receive_buffer) - 1, pdMS_TO_TICKS(100));
        
        if (rxBytes > 0) 
        {
            receive_buffer[rxBytes] = '\0'; // Null-terminate the string
            
            // Output to the Serial Monitor via Native USB
            Serial.printf("Success! Read via UART1: %s\n", receive_buffer);
        } 
        else 
        {
            Serial.println("Nothing received. Have you connected the TX and RX pins?");
        }

        // Delay for 1 second before the next cycle
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}