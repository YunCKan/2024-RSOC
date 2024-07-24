/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 * 2024-07-23     YunCKan      create two threads
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

int main(void)
{
/*
    while (1)
    {
		thread_sample(); 
		rt_thread_mdelay(500);
    }
    */
    thread_sample(); 
	return 0;
}