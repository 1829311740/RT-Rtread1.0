#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include "pwm1.h"
#include "uart2.h"
#define BUFFER_SIZE 8
rt_device_t uart_dev3;
struct serial_configure uart_config3 = RT_SERIAL_CONFIG_DEFAULT;

rt_uint8_t recv_buffer3[BUFFER_SIZE]; // 用于存储接收到的字节
static rt_sem_t rx_sem3 = RT_NULL;

// 声明 uart2_send_data 函数
extern void uart2_send_data(void);
extern void uart2_send2_data(void);
static rt_err_t uart3_input(rt_device_t dev, rt_size_t size) {
    rt_sem_release(rx_sem3); // 释放信号量，表示接收到数据
    return RT_EOK;
}

static void uart3_rx_thread_entry(void *parameter) {
    while (1) {
        // 等待信号量，表示接收到数据
        if (rt_sem_take(rx_sem3, RT_WAITING_FOREVER) == RT_EOK) {
            // 从 UART 读取数据
            rt_size_t size = rt_device_read(uart_dev3, 0, recv_buffer3, sizeof(recv_buffer3));
            if (size > 0) {
                // 打印接收到的数据
                rt_kprintf("Received data:");
                for (int i = 0; i < size; i++) {
                    rt_kprintf(" %c", recv_buffer3[i]); // 直接打印字符
                    // 判断是否接收到字符 '2'
                    if (recv_buffer3[i] == '7') {
                        // 调用 uart2 发送数据函数
                        uart2_send_data();
                    }
                    if (recv_buffer3[i] == '8') {
                        // 调用 uart2 发送数据函数
                        uart2_send2_data();
                    }
                    if (recv_buffer3[i] == '1') {
                            kai();
                    }
                    if (recv_buffer3[i] == '2') {
                            guan();
                    }
                    if (recv_buffer3[i] == '3') {
                            rt_pin_write(GET_PIN(F, 4), PIN_HIGH);
                     }
                    if (recv_buffer3[i] == '4') {
                            rt_pin_write(GET_PIN(F, 5), PIN_HIGH);
                      }
                     if (recv_buffer3[i] == '5') {
                            rt_pin_write(GET_PIN(F, 6), PIN_HIGH);
                    }
                      if (recv_buffer3[i] == '6') {
                            rt_pin_write(GET_PIN(F, 7), PIN_HIGH);
                    }
                }
                rt_kprintf("\n");
            }
        }
    }
}

int uart3() {
    led();
    rt_int8_t result = 0;

    uart_dev3 = rt_device_find("uart3");
    if (uart_dev3 == RT_NULL) {
        rt_kprintf("find error\n");
        return -RT_ERROR;
    }

    result = rt_device_open(uart_dev3, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if (result < 0) {
        rt_kprintf("open error\n");
        return result;
    }

    rt_device_control(uart_dev3, RT_DEVICE_CTRL_CONFIG, (void *)&uart_config3);

    // 创建信号量
    rx_sem3 = rt_sem_create("rx_sem3", 0, RT_IPC_FLAG_FIFO);
    if (rx_sem3 == RT_NULL) {
        rt_kprintf("sem create error\n");
        return -RT_ERROR;
    }

    rt_device_set_rx_indicate(uart_dev3, uart3_input);

    // 创建接收线程
    rt_thread_t thread = rt_thread_create("uart3_rx", uart3_rx_thread_entry, RT_NULL, 1024, 25, 10);
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    } else {
        rt_kprintf("thread create error\n");
        return -RT_ERROR;
    }

    return RT_EOK;
}

INIT_APP_EXPORT(uart3);
