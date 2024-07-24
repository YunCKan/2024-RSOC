/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)
#define PIN_KEY0 GET_PIN(C, 0) // PC0: KEY0 --> KEY

#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

/* thread handler */
static rt_thread_t myThread1 = RT_NULL;
static rt_thread_t myThread2 = RT_NULL;

static rt_sem_t dynamic_sem = RT_NULL;
static rt_sem_t dynamic_sem_off = RT_NULL;

/* thread #1 entry function */
static void t1_entry(void *parameter);   //按键线程
static void t2_entry(void *parameter);


int main(void)
{
    rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_KEY0, PIN_MODE_INPUT_PULLUP);
    
    dynamic_sem = rt_sem_create("dsem", 0, RT_IPC_FLAG_PRIO);
    if (dynamic_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }
    else
    {
        rt_kprintf("create done. dynamic semaphore value = 0.\n");
    }
    dynamic_sem_off = rt_sem_create("off", 0, RT_IPC_FLAG_PRIO);
    if (dynamic_sem_off == RT_NULL)
    {
        rt_kprintf("create off semaphore failed.\n");
        return -1;
    }
    else
    {
        rt_kprintf("create done off semaphore value = 0.\n");
    }

	myThread1 = rt_thread_create("thread1",
                            t1_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
	if (myThread1 != RT_NULL)
        rt_thread_startup(myThread1);

	myThread2 = rt_thread_create("thread2",
                            t2_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);
	if (myThread2 != RT_NULL)
        rt_thread_startup(myThread2);					
    /*
    while (1)
    {
        rt_pin_write(GPIO_LED_R, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED_R, PIN_LOW);
        rt_thread_mdelay(500);
    }
    */
	return 0;
}

static void t1_entry(void *parameter)
{
    rt_uint32_t count = 0;

    while (1)
    {
        /* 读 取 按 键 KEY0 的 引 脚 状 态 */
        if (rt_pin_read(PIN_KEY0) == PIN_LOW)
        {
            rt_thread_mdelay(100);
            if (rt_pin_read(PIN_KEY0) == PIN_LOW)
            {
                //按 键 已 被 按 下， 输 出 log 点 亮 LED 灯,释放信号量
                rt_kprintf("KEY0 pressed\n");
                rt_sem_release(dynamic_sem);
            }
        }
        else
        {
            rt_thread_mdelay(100);
            if (rt_pin_read(PIN_KEY0) == PIN_HIGH)
            {
                //按 键 已 被 按 下， 输 出 log 点 亮 LED 灯,释放信号量
                rt_kprintf("KEY0 released\n");
                rt_sem_release(dynamic_sem_off);
            }
        }
    }
}
static void t2_entry(void *parameter)
{
    rt_uint32_t result = 0;
    rt_uint32_t result_off = 0;

    while (1)
    {
        result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        /* 读 取 按 键 KEY0 的 引 脚 状 态 */
        if (result == RT_EOK)
        {
                //按 键 已 被 按 下， 输 出 log 点 亮 LED 灯
                rt_kprintf("led off\n");
                rt_pin_write(GPIO_LED_R, PIN_HIGH);
                rt_thread_mdelay(2000);
        }

        result_off = rt_sem_take(dynamic_sem_off, RT_WAITING_FOREVER);
        if (result_off == RT_EOK)
        {
                //按 键 已 被 按 下， 输 出 log 点 亮 LED 灯
                rt_kprintf("led on\n");
                rt_pin_write(GPIO_LED_R, PIN_LOW);
                rt_thread_mdelay(2000);
        }
    }
}


int LEDR_On(void)
{
	rt_kprintf("ledr on\n");
	rt_thread_mdelay(2000);
	return 0;
}
int LEDR_Off(void)
{
	rt_kprintf("ledr off\n");
	rt_thread_mdelay(2000);
	return 0;
}

MSH_CMD_EXPORT(LEDR_On, thread sample);
MSH_CMD_EXPORT(LEDR_Off, thread sample);


