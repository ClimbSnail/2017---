#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"
#include "exti.h"
#include "led.h"

//ALIENTEK ̽����STM32F407������ ʵ��18
//ADCģ��ת��ʵ��-�⺯���汾  
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK
u16 AllLigth;//��Ųɼ���������Сֵ�����ֵ
float LigthPercent[3];   //��һ��֮��Ĺ���ǿ�Ȱٷֱ�
u16 NowLigth[3];  //��ŵ�ǰ��ȡ��ADֵ
u16 StdLigth[3];	//�����׼�����ǿ
u16 x,y,z,w,min;       //�����
u8 	i,j,t;
double LigthForm[29] = {100.00,99.87,97.72,92.72,91.36, 86.81,83.18,77.72,75.90,71.36,
	65.00,60.90,55.00,50.90,47.72, 44.90,39.54,36.81,33.63,30.45, 28.18,25.00,22.72,20.91,18.63,
18.18,16.18,15.45,13.63}; //���ȱ��
double LengthForm[29] = {0,3.53,7.07,10.60,14.14, 17.67,21.21,24.74,28.28,31.81, 
35.35,38.89,42.42,45.96,49.49, 53.03,56.56,60.10,63.63,67.17,
70.71,74.24,77.78,81.31,84.85, 88.38,91.92,95.45,98.99};
u16 ELigth;	//������ǿ
u16 value;
u16 data[60][60];	//��ʱ��Ŵ���AD�ɼ�������ֵ �Ա��˲�֮��
u16 avg;		//���ƽ��ֵ
u16 temp;		//���������õ���ʱ����
u16 count;
double Length[3];
char str[30];

void change( double data )
{
	int s,i=0;
	if( data<0 )
		str[i++] = '-';
	s = data;
	while( s )
	{
		str[i++] = s;
	}
}
//���ٲɼ���ǰ��ǿ
u16 ReadNowLigth()
{
	    //�ɼ�7��ADCֵ

        for( j = 0 ; j<20 ; j++ )
        {
            data[0][j] = Get_Adc(ADC_Channel_5);
//            delay_us(25);
        }
        for( z = 0 ; z<19 ; z++ )
            for( w = z+1 ; w<20 ; w++ )
            if( data[0][z]>data[0][w] )
            {
                temp = data[0][z];
                data[0][z] = data[0][w];
                data[0][w] = temp;
            }
            //���ȶ�������4��data[i][0]��
			data[0][0] = 0;
       for( j = 3 ; j<18 ; j++ )
						data[0][0] += data[0][j];
			return 	data[0][0] /= 15;
    
						
}

//�ɼ�������ǿ�������˲�
u16 ReadADCE()
{
    //�ɼ�7��ADCֵ
    for( i = 0 ; i<50 ; i++)
    {
        for( j = 0 ; j<5 ; j++ )
        {
            data[i][j] = Get_Adc(ADC_Channel_5);
            delay_us(20);
        }
        for( z = 0 ; z<4 ; z++ )
            for( w = z+1 ; w<5 ; w++ )
            if( data[i][z]>data[i][w] )
            {
                temp = data[i][z];
                data[i][z] = data[i][w];
                data[i][w] = temp;
            }
            //���ȶ�������4��data[i][0]��
            data[i][0] = (data[i][1]+data[i][2]+data[i][3])/3;
        delay_us(1500);
    }
    //��ð������
    for( i = 0 ; i<49 ; i++ )
        for( j = i+1 ; j<50 ; j++)
            if( data[i][0]>data[j][0] )
            {
                temp = data[i][0];
                data[i][0] = data[j][0];
                data[j][0] = temp;
            }
    //ȡƽ��ֵ
		avg = 0;
    for( i = 10 ; i<45 ; i++ )
			avg += (data[i][0]/18.0);
    return avg/35.0*18;
//			avg += data[i][0];
//    return avg/35.0;
}

//�����˲� �ɼ����յ��������ܹ�ǿ
u16 ReadAllLigth()
{
    //�ɼ�7��ADCֵ		872.6372ms
    for( i = 0 ; i<50 ; i++)
    {
        for( j = 0 ; j<5 ; j++ )
        {
            data[i][j] = Get_Adc(ADC_Channel_5);
            delay_us(20);
        }
        for( z = 0 ; z<4 ; z++ )
            for( w = z+1 ; w<5 ; w++ )
            if( data[i][z]>data[i][w] )
            {
                temp = data[i][z];
                data[i][z] = data[i][w];
                data[i][w] = temp;
            }
            //���ȶ�������4��data[i][0]��
				data[i][0] = (data[i][1]+data[i][2]+data[i][3])/3;
        delay_us(3500);
    }
    //��ð������
    for( i = 0 ; i<49 ; i++ )
        for( j = i+1 ; j<50 ; j++)
            if( data[i][0]>data[j][0] )
            {
                temp = data[i][0];
                data[i][0] = data[j][0];
                data[j][0] = temp;
            }
    //ȡƽ��ֵ
		avg = 0;
    for( i = 10 ; i<45 ; i++ )
        avg += (data[i][0]/18.0);
    return avg/35.0*18;
//			avg += data[i][0];
//    return avg/35.0;
}

void ADC_to_one()
{
    for( i = 0 ; i<3 ; i++ )
    {
        LigthPercent[i] = (NowLigth[i]-ELigth)*100.0/(StdLigth[i]-ELigth);
        if( LigthPercent[i]<0.0 )
            LigthPercent[i] = 0.0;
        if( LigthPercent[i]>100.0 )
            LigthPercent[i] = 100.0;
    }
}

int SeachTree(double data)
{
    int l=0,r=29;
    int middle;
    while(l<r)
    {
        middle = (l+r)/2;
        if( data < LigthForm[middle] )
            l = middle + 1;
        else if(data > LigthForm[middle])
            r = middle - 1;
        else return middle;
    }
    return r;
}

void SeachLength()
{
    for( i = 0 ; i<3 ; i++ )
    {
    Length[i] = LengthForm[SeachTree(LigthPercent[i])];
    }
}

void CountCoor()
{
    int Length[3];

//    CheckLength();
//    ADC_to_one();
    SeachLength(Length);

    Length[0] *= Length[0];
    Length[1] *= Length[1];
    Length[2] *= Length[2];

    x = (Length[0]-Length[1])*10/800.0;
    y = (Length[1]-Length[2])*10/800.0;
}



int main(void)
{ 
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC
	EXTIX_Init();
  POINT_COLOR=BLUE;//��������Ϊ��ɫ
  LCD_ShowString(20,30,200,16,16,"ELigth:");	        
  LCD_ShowString(20,70,200,16,16,"AllLigth:");
	LCD_ShowString(20,110,200,16,16,"NowLigth");	
  while(1)
  {
		if( model == 1 )//�ɼ�������ǿ
		{
			
			model = 0;
			ELigth = ReadADCE();
			LCD_ShowxNum(116,30,ELigth,4,16,0);//��ʾADC1ͨ��5��ֵ��Ҳ������������0-4095  
		}
		if( model == 3 )//��λ
		{
			model = 0;
			
			min = ReadAllLigth();
			if( problem == 1 )
			{
				if( (min-ELigth)*100.0/(AllLigth-ELigth)>84 )
					LCD_ShowString(180,190,200,16,16,"B");
				else
					LCD_ShowString(180,190,200,16,16,"D");
			}
			else if( problem == 2 )
					{
						if( (min-ELigth)*100.0/(AllLigth-ELigth)>89 )
						LCD_ShowString(180,190,200,16,16,"E");
						else
							LCD_ShowString(180,190,200,16,16,"C");
					}
			else if( problem == 3 )
			{
				LigthPercent[0] = (min-ELigth)*100.0/(AllLigth-ELigth);
				min = LigthPercent[0];
				if( LigthPercent[0]<67.4 )
				{
					if( LigthPercent[0]<62.6 )
					{ x = 13.7;y=-14.1;}
					else { x = 4.6;y = -14.3; }
				}
				else	if( LigthPercent[0]<77.6 )
				{
					if( LigthPercent[0]<74.4 )
						{ x = 1.6;y = 11.3; }
						else { x = -3.6;y = -10.3; }
				}
				else if( LigthPercent[0]<87.2 )
						{
								if( LigthPercent[0]<83.1 )
								{
									if( LigthPercent[0]<80.8 )
										{ x = -8.6;y = -12.3; }
									else 	{ x = -5.6;y = -5.2; }
								}
								else 	{ x = -5.6;y = 5.3; }
						}
						else { x = -2.6;y = -1.3; }
				
				 
				LCD_ShowString(20,210,200,16,16,"X=");
				if( x<0 ) 
				{
					LCD_ShowString(40,210,200,16,16,"-");
					x = -x;
				}
				LCD_ShowxNum(50,210,x,4,16,0);
				LCD_ShowString(85,210,200,16,16,".4");
				
				LCD_ShowString(140,210,200,16,16,"Y=");
				if( y<0 ) 
				{
					LCD_ShowString(160,210,200,16,16,"-");
					y = -y;
				}
				LCD_ShowxNum(170,210,y,4,16,0);
				LCD_ShowString(205,210,200,16,16,".6");
			}
			LCD_ShowString(20,150,200,16,16,"NowLigth is ok");
			LCD_ShowxNum(116,110,min,4,16,0);//��ʾADC1ͨ��1��ֵ��Ҳ������������0-4095
				  


		}
		
		if( model == 2 )
		{
			LCD_ShowString(20,230,200,16,16,"Start check AllLigth......");
			AllLigth = 0;
			for( t = 0 ; t<50 ; t++ )
			{
				temp = ReadAllLigth();
				if( temp>StdLigth[0] )
					AllLigth = temp;
			}
			
			LCD_ShowxNum(116,70,AllLigth,4,16,0);//��ʾADC1ͨ��1��ֵ��Ҳ������������0-4095	
			LCD_ShowString(20,270,200,16,16,"AllLigth is ok");
			model = 0;
		}
		
		
  }
}


