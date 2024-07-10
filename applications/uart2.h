#ifndef UART2_H
#define UART2_H

extern rt_device_t uart_dev;  // 声明全局变量
extern rt_int8_t recv_buffer[20];
int uart2();  // 声明函数
int led();

#endif /* UARTFDS_H */
