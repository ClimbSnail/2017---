#include "timer.h"
#include "led.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "oled.h"

float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
short temp;					//�¶�
u8 start;
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001) //����ж�
	{
		start = 1;
	}			
	TIM2->SR&=~(1<<0); //����жϱ�־λ
}
//ͨ�ö�ʱ�� 2 �жϳ�ʼ��
//����ʱ��ѡ��Ϊ APB1 �� 2 ������ APB1 Ϊ 36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ�� 2!
void TIM2_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0; //TIM2 ʱ��ʹ��
	TIM2->ARR=arr; //�趨�������Զ���װֵ//�պ� 1ms
	TIM2->PSC=psc; //Ԥ��Ƶ�� 7200,�õ� 10Khz �ļ���ʱ��
	TIM2->DIER|=1<<0; //��������ж�
//	TIM2->CR1|=0x01; //ʹ�ܶ�ʱ�� 2

	MY_NVIC_Init(1,0,TIM2_IRQn,2);//��ռ 1�������ȼ� 3���� 2
}


//PWM �����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{
		//�˲������ֶ��޸� IO ������
		RCC->APB1ENR|=1<<11;      //TIM1 ʱ��ʹ��
		RCC->APB2ENR|=1<<2; //ʹ�� PORTA ʱ��
	
		GPIOA->CRH&=0XFFFFFFF0; //PA8 ���
		GPIOA->CRH|=0X0000000B; //���ù������
		GPIOA->CRH&=0XFFFFFF0F; //PA9 ���
		GPIOA->CRH|=0X000000B0; //���ù������
		GPIOA->CRH&=0XFFFFF0FF; //PA10 ���
		GPIOA->CRH|=0X00000B00; //���ù������
		GPIOA->CRH&=0XFFFF0FFF; //PA11 ���
		GPIOA->CRH|=0X0000B000; //���ù������
	
	
		RCC->APB2ENR|=1<<0; //��������ʱ��
	
		AFIO->MAPR&=0XFFFFFF3F; //��� MAPR ��[7:6]
//		AFIO->MAPR|=0<<6; //û����ӳ��,TIM1_CH1N->PB13
		TIM1->ARR=arr; //�趨�������Զ���װֵ
		TIM1->PSC=psc; //Ԥ��Ƶ������Ƶ
		
		TIM1->CCMR1|=7<<4; //CH1 PWM2 ģʽ
		TIM1->CCMR1|=7<<12; //CH2 PWM2 ģʽ
		TIM1->CCMR2|=7<<4; //CH3 PWM2 ģʽ
		TIM1->CCMR2|=7<<12; //CH4 PWM2 ģʽ
		
		TIM1->CCMR1|=1<<3; //CH1 Ԥװ��ʹ��	
		TIM1->CCMR1|=1<<11; //CH2 Ԥװ��ʹ��
		TIM1->CCMR2|=1<<3; //CH3 Ԥװ��ʹ��	
		TIM1->CCMR2|=1<<11; //CH4 Ԥװ��ʹ��
		
		TIM1->CCER|=1<<0; //OC1 ���ʹ��
		TIM1->CCER|=1<<4; //OC2 ���ʹ��
		TIM1->CCER|=1<<8; //OC3 ���ʹ��
		TIM1->CCER|=1<<12; //OC4 ���ʹ��
		
		TIM1->CR1=0x0080; //ARPE ʹ��
		
		TIM1->CR1|=0x01; //ʹ�ܶ�ʱ�� 1	
}

//PWM �����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{
		//�˲������ֶ��޸� IO ������
		RCC->APB1ENR|=1<<1;      //TIM3 ʱ��ʹ��
		RCC->APB2ENR|=1<<3; //ʹ�� PORTB ʱ��
	
		GPIOB->CRL&=0XFFFFFFF0; //PB0 ���
		GPIOB->CRL|=0X0000000B; //���ù������
		GPIOB->CRL&=0XFFFFFF0F; //PB1 ���
		GPIOB->CRL|=0X000000B0; //���ù������
		GPIOA->CRL&=0X0FFFFFFF; //PA7 ���
		GPIOA->CRL|=0XB0000000; //���ù������

	
		RCC->APB2ENR|=1<<0; //��������ʱ��

		AFIO->MAPR&=0XFFFFF3FF; //��� MAPR ��[11:10]  û����ӳ��
		TIM3->ARR=arr; //�趨�������Զ���װֵ
		TIM3->PSC=psc; //Ԥ��Ƶ������Ƶ
		

		TIM3->CCMR1|=7<<12; //CH2 PWM2 ģʽ
		TIM3->CCMR2|=7<<4; //CH3 PWM2 ģʽ
		TIM3->CCMR2|=7<<12; //CH4 PWM2 ģʽ
		
		TIM3->CCMR1|=1<<11; //CH2 Ԥװ��ʹ��
		TIM3->CCMR2|=1<<3; 	//CH3 Ԥװ��ʹ��	
		TIM3->CCMR2|=1<<11; //CH4 Ԥװ��ʹ��
		

		TIM3->CCER|=1<<4; //OC2 ���ʹ��
		TIM3->CCER|=1<<8; //OC3 ���ʹ��
		TIM3->CCER|=1<<12; //OC4 ���ʹ��
		
		TIM3->CR1=0x0080; //ARPE ʹ��
		
		TIM3->CR1|=1 << 0; //ʹ�ܶ�ʱ�� 1	
}
