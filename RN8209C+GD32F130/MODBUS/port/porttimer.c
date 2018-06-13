/*
 * FreeModbus Libary: MSP430 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.3 2007/06/12 06:42:01 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
/* Timer ticks are counted in multiples of 50us. Therefore 20000 ticks are
 * one second.
 */
#define MB_TIMER_TICKS          ( 20000L )

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timeout50us )
{  
      
    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER13);

    timer_deinit(TIMER13);

    /* TIMER2 configuration */
    timer_initpara.prescaler         = 7199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = usTim1Timeout50us-1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER13,&timer_initpara);
    
    timer_auto_reload_shadow_enable(TIMER13);

//    timer_auto_reload_shadow_enable(TIMER13);
    timer_interrupt_flag_clear(TIMER13, TIMER_INT_FLAG_UP); 
	  timer_autoreload_value_config(TIMER13,0);
    timer_interrupt_enable(TIMER13, TIMER_INT_FLAG_UP);
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER13_IRQn, 1, 4);
    timer_init(TIMER13,&timer_initpara);
		//timer_enable(TIMER13);


  return TRUE;
}
//开启定时器
void
vMBPortTimersEnable( void )
{
	  //timer_auto_reload_shadow_enable(TIMER13);
    //timer_interrupt_flag_clear(TIMER13, TIMER_INT_FLAG_UP); 
	  //timer_autoreload_value_config(TIMER13,0);
    //timer_interrupt_enable(TIMER13, TIMER_INT_FLAG_UP);
	  timer_enable(TIMER13);

}
//关闭定时器
void
vMBPortTimersDisable( void )
{
	//timer_autoreload_value_config(TIMER13,0);
  //timer_interrupt_disable(TIMER13,TIMER_INT_UP);
	timer_disable(TIMER13);
}


void prvvTIMERExpiredISR( void )
{
	( void )pxMBPortCBTimerExpired();
}
