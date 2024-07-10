#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#define PWM_DEV_NAME        "pwm4"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */      //pb6
rt_uint32_t period, pulse;
struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */
int pwm(void){

         period = 20000000; /* 20ms的周期，单位为纳秒ns */

         /* 查找设备 */
         pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
         if (pwm_dev == RT_NULL)
         {
             rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
             return RT_ERROR;
         }
         /* 使能设备 */
         rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
         return 0;
}

int guan(void){
    pulse = 2500000; /* 转到0度位置 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    return 0;
}
//rt_thread_mdelay(1000); /* 延时1秒 */
int kai(void){
    pulse = 1000000; /* 转到180度位置 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    return 0;
}






















