#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
static int flag=0; 
	
void EXTIX_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_B,7,FTIR); //�½��ش���
	Ex_NVIC_Config(GPIO_B,6,FTIR); //�½��ش���
	Ex_NVIC_Config(GPIO_B,5,FTIR); //�½��ش���
	Ex_NVIC_Config(GPIO_B,4,FTIR); //�½��ش���
	
	MY_NVIC_Init(2,3,EXTI9_5_IRQn,2); //��ռ 2�������ȼ� 1���� 2
	MY_NVIC_Init(2,0,EXTI4_IRQn,2); //��ռ 2�������ȼ� 0���� 2
}

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
	RCC->APB2ENR|=1<<3; //ʹ�� PORTB ʱ��
	RCC->APB2ENR|=1<<0; //ʹ�� ���Ÿ��� ʱ��
	
	GPIOB->CRL&=0X0FFFFFFF; //PB7 ���ó����룬Ĭ������
	GPIOB->CRL|=0X80000000;
	GPIOB->CRL&=0X0FFFFFFF; //PB6 ���ó����룬Ĭ������
	GPIOB->CRL|=0X80000000;
	GPIOB->CRL&=0X0FFFFFFF; //PB5 ���ó����룬Ĭ������
	GPIOB->CRL|=0X80000000;
	GPIOB->CRL&=0X0FFFFFFF; //PB4 ���ó����룬Ĭ������
	GPIOB->CRL|=0X80000000;
	GPIOB->ODR|= 15 << 4;  //   ȫ������
	

}


u8 KEY_Scan(void)
{	  
	if((KEY1==0||KEY2==0||KEY3==0||KEY4==1))
	{
		delay_ms(10);//ȥ���� 
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==1)return KEY4_PRES;
	} 	
 	return 0;// �ް�������
}

void EXTI9_5_IRQHandler()
{
	
	delay_ms(50); //����
	if(KEY1==0)
	{		
		TIM2->CR1 &= ~0x01; //�رն�ʱ�� 2
		TIM2->CNT = 0;
		if( flag == 0 )
		{
		LED1 = 0;
		LED2 = 0;
		LED3 = 0;
		}
		else if( flag == 1 )
				{
					LED1 = 1;
					LED2 = 0;
					LED3 = 0;
				}
				else if( flag == 2 )
				{
					LED1 = 0;
					LED2 = 1;
					LED3 = 0;
				}
					else if( flag == 3 )
					{
						LED1 = 0;
						LED2 = 0;
						LED3 = 1;
					}
					else if( flag == 4 )
					{
						LED1 = 1;
						LED2 = 1;
						LED3 = 1;
					}
		start = 0;
		LED4 = 1;
		flag = (flag+1)%5;
		delay_ms(50); //����
		while(!KEY1);
					
	}
	else if(KEY2==0)
	{
		TIM2->CNT = 0;
		TIM2->CR1|=0x01; //ʹ�ܶ�ʱ�� 2
		
		LED4 = 0;
		while(!KEY2);
		start = 1;
	}
	else if(KEY3==0)
	{	
		
		
		while(!KEY3);	

	}
	EXTI->PR=1<<7; //��� LINE7 �ϵ��жϱ�־λ
	EXTI->PR=1<<6; //��� LINE6 �ϵ��жϱ�־λ		
	EXTI->PR=1<<5; //��� LINE5 �ϵ��жϱ�־λ
}

void EXTI4_IRQHandler()
{
	delay_ms(20); //����
	if( KEY4 == 0 )
	{
		
		
		while(!KEY4);	
		EXTI->PR|= 1<<4;  //��� LINE4 �ϵ��жϱ�־λ
	}
	EXTI->PR|= 1<<4;  //��� LINE4 �ϵ��жϱ�־λ
}





