#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include "pwm1.h"

#define BUFFER_SIZE 8
#define keting GET_PIN(F, 4) // PF11 : LED_B --> LED
#define woshi GET_PIN(F, 5) // PF12 : LED_R --> LED
#define chufang GET_PIN(F, 6) // PF11 : LED_B --> LED
#define weishenjian GET_PIN(F, 7) // PF12 : LED_R --> LED
#define fs GET_PIN(A, 0)

int led() {
    rt_pin_mode(keting, PIN_MODE_OUTPUT);
    rt_pin_mode(woshi, PIN_MODE_OUTPUT);
    rt_pin_mode(chufang, PIN_MODE_OUTPUT);
    rt_pin_mode(weishenjian, PIN_MODE_OUTPUT);
    rt_pin_mode(fs, PIN_MODE_OUTPUT);

    return 0;
}

rt_device_t uart_dev;
struct serial_configure uart_config = RT_SERIAL_CONFIG_DEFAULT;

rt_uint8_t recv_buffer[BUFFER_SIZE]; // 用于存储接收到的字节
static rt_sem_t rx_sem = RT_NULL;


void uart2_send2_data(void) {
    rt_uint8_t tx_buffer[] = {0xAA, 0x55, 0x02, 0x00, 0x55, 0xAA};
    rt_device_write(uart_dev, 0, tx_buffer, sizeof(tx_buffer));
}

void uart2_send_data(void) {
    rt_uint8_t tx_buffer[] = {0xAA, 0x55, 0x01, 0x00, 0x55, 0xAA};
    rt_device_write(uart_dev, 0, tx_buffer, sizeof(tx_buffer));
}

static rt_err_t uart_input(rt_device_t dev, rt_size_t size) {
    rt_sem_release(rx_sem); // 释放信号量，表示接收到数据
    return RT_EOK;
}

static void uart_rx_thread_entry(void *parameter) {
    while (1) {
        // 等待信号量，表示接收到数据
        if (rt_sem_take(rx_sem, RT_WAITING_FOREVER) == RT_EOK) {
            // 从 UART 读取数据
            rt_size_t size = rt_device_read(uart_dev, 0, recv_buffer, sizeof(recv_buffer));
            if (size > 0) {
                if (recv_buffer[0] == 0xF0) {
                    kai();
                } else if (recv_buffer[0] == 0xF1) {
                    guan();
                } else if (recv_buffer[0] == 0xA0) {
                    rt_pin_write(keting, PIN_HIGH);
                } else if (recv_buffer[0] == 0xA1) {
                    rt_pin_write(keting, PIN_LOW);
                } else if (recv_buffer[0] == 0xB0) {
                    rt_pin_write(woshi, PIN_HIGH);
                } else if (recv_buffer[0] == 0xB1) {
                    rt_pin_write(woshi, PIN_LOW);
                } else if (recv_buffer[0] == 0xC0) {
                    rt_pin_write(weishenjian, PIN_HIGH);
                } else if (recv_buffer[0] == 0xC1) {
                    rt_pin_write(weishenjian, PIN_LOW);
                } else if (recv_buffer[0] == 0xD0) {
                    rt_pin_write(chufang, PIN_HIGH);
                } else if (recv_buffer[0] == 0xD1) {
                    rt_pin_write(chufang, PIN_LOW);
                }
            }
        }
    }
}

int uart2() {
    pwm();
    led();
    rt_int8_t result = 0;

    uart_dev = rt_device_find("uart2");
    if (uart_dev == RT_NULL) {
        rt_kprintf("find error\n");
        return -RT_ERROR;
    }

    result = rt_device_open(uart_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    if (result < 0) {
        rt_kprintf("open error\n");
        return result;
    }

    rt_device_control(uart_dev, RT_DEVICE_CTRL_CONFIG, (void *)&uart_config);

    // 创建信号量
    rx_sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (rx_sem == RT_NULL) {
        rt_kprintf("sem create error\n");
        return -RT_ERROR;
    }

    rt_device_set_rx_indicate(uart_dev, uart_input);

    // 创建接收线程
    rt_thread_t thread = rt_thread_create("uart_rx", uart_rx_thread_entry, RT_NULL, 1024, 25, 10);
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    } else {
        rt_kprintf("thread create error\n");
        return -RT_ERROR;
    }

    return RT_EOK;
}

INIT_APP_EXPORT(uart2);
