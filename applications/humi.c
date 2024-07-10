#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "aht10.h"
#define DBG_TAG "aht10"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
int humidity,temperature;
aht10_device_t dev;
int ahtt(void)
{
    /* 总线名称 */
    const char *i2c_bus_name = "i2c3";
    //int count = 0;

    /* 等待传感器正常工作 */
    rt_thread_mdelay(2000);

    /* 初始化 aht10 */
    dev = aht10_init(i2c_bus_name);
    if (dev == RT_NULL)
    {
        LOG_E(" The sensor initializes failure");
        //return 0;
    }
    humidity = aht10_read_humidity(dev);
    temperature = aht10_read_temperature(dev);


    return 0;
}


int ahtt1(void){
    humidity = aht10_read_humidity(dev);
    return humidity ;
}
int ahtt2(void){
    temperature = aht10_read_temperature(dev);
    return temperature;
}
















