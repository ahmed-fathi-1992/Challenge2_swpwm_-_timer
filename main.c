/*
 * gpio.c
 *
 * Created: 17/01/2020 14:44:58
 *  Author: ahmed_pc
 */ 


typedef enum prog {
  seg_counter,
  Button_press,
  Traffic_light,
  Motor,
  Test,
}prog;

typedef enum{
	GO_state=0,Stop_state,Get_Ready_state
}prog_states;

void seg_counter_fun(void);
void Button_press_fun(void);
void Traffic_light_fun(void);
void MOTOR(void);
void Test_1(void);

//#include <avr/io.h>
//#include "registers.h"
#include "gpio.h"
#include "led.h"
#include "softwareDelay.h"
#include "pushButton.h"
#include "sevenSeg.h"
#include "timers.h"
#include "interrupt.h"
#include "std_types.h"
//#include "Motor.h"
#include "dcMotor.h"


uint32_t spwm_duty_cycle=0;
EN_bits_t spwm_pin_1= MOTOR_EN_1_BIT;
EN_bits_t spwm_pin_2= MOTOR_EN_2_BIT;

int main(void)
{
	prog program = Motor; // select program to run GPIO_REQ7:(seg_counter) GPIO_REQ8:(Button_press) GPIO_REQ9:(Traffic_light) GPIO_REQ15:(MOTOR)
 switch (program)
  {
		case seg_counter:
		 seg_counter_fun();				
		break;
		
		case Button_press:
		 Button_press_fun();
		break;
		
		case Traffic_light:
		 Traffic_light_fun();		
		break;
		
		case Motor:
		MOTOR();
		break;
		
		case Test:
		Test_1();
		break;

		default:
		break;
 }
	
}


	
void seg_counter_fun(void)
{
			sevenSegInit(SEG_0);
			sevenSegInit(SEG_1);
			while(1)
			{
				//softwareDelayMs(1000);
				timer0DelayMs(1000);
				for (int digit_1 =0 ;digit_1<10 ;digit_1++)
				{
					for(int digit_0 =0 ;digit_0<10 ;digit_0++)
					{
						for (int count =0 ;count<100 ;count++)
						{
							
							sevenSegWrite(SEG_0, digit_0);
							timer0DelayMs(5);
							//softwareDelayMs(5);
							sevenSegDisable( SEG_0);
							
							sevenSegWrite(SEG_1, digit_1);
							timer0DelayMs(5);
							//softwareDelayMs(5);
							sevenSegDisable( SEG_1);
						}
					}
				}

			}	
}

void Button_press_fun(void)
{
	uint8_t led_on_repeat=0;
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64, 0, 0, T0_INTERRUPT_NORMAL);
	pushButtonInit(BTN_1);
	Led_Init(LED_1);
	while(1)
	{  led_on_repeat=0;
		if( Prepressed== pushButtonGetStatus(BTN_1))
		{
			while(Prepressed== pushButtonGetStatus(BTN_1));
			Led_On(LED_1);
			for (int counter=0; counter<20;counter++)
			{
				if( Prepressed== pushButtonGetStatus(BTN_1))
				led_on_repeat++;
												
			}
			if( led_on_repeat>0) {timer0DelayMs(1000);} //softwareDelayMs(1000);
			
			Led_Off(LED_1);
			
		}
		
		
	}
	
}

void Traffic_light_fun(void)
{
	prog_states state = GO_state;
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64, 0, 0, T0_INTERRUPT_NORMAL);
	Led_Init(LED_1); Led_Init(LED_2); Led_Init(LED_3);
	Led_Off(LED_1);  Led_Off(LED_2);  Led_Off(LED_3);
	//	Led_On(LED_1);   Led_On(LED_2);   Led_On(LED_3);
	
	while(1){
		switch(state) {
			case GO_state:        {Led_On(LED_1); timer0DelayMs(1000); Led_Off(LED_1);     state=Stop_state;       break;}
			case Stop_state:      {Led_On(LED_3); timer0DelayMs(1000); Led_Off(LED_3);     state=Get_Ready_state;  break;}
			case Get_Ready_state: {Led_On(LED_2); timer0DelayMs(1000); Led_Off(LED_2);     state=GO_state;         break;}
		}//end of switch.
	}//end of while	
	
}


void MOTOR(void)
{

int speed=0;

MotorDC_Init(MOT_1);
MotorDC_Init(MOT_2);
timer2Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64, 0, 0, 0,T0_POLLING); // for delay

SREG= SREG |0x80 ;// enable global interrupt

	
	MotorDC_Dir(MOT_1, FORWARD);
	MotorDC_Dir(MOT_2, FORWARD);
	
	for (speed=0; speed<100;speed++)  // 5 second from %0 to %100
	{
		timer2DelayMs(50);
		MotorDC_Speed_PollingWithT0(speed);
	}
	for (speed=100; speed>0;speed--) // 5 second from %100 to %0
	{
		timer2DelayMs(50);
		MotorDC_Speed_PollingWithT0(speed);
	}
	
		MotorDC_Dir(MOT_1, BACKWARD);  //Motor_direction(Right);
		MotorDC_Dir(MOT_2, FORWARD);

	
	for (speed=0; speed<85;speed++) // .5 second to turn right 
	{
		timer2DelayMs(10);
		MotorDC_Speed_PollingWithT0(25);
	}


	MotorDC_Dir(MOT_1, STOP);
	MotorDC_Dir(MOT_2, STOP);
	
while(1);
//{}


}


		



void Test_1(void)
{
  //timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64, 0, 0, T0_INTERRUPT_NORMAL);
  //timer1Init(T1_NORMAL_MODE,T1_OC1_DIS,T1_PRESCALER_NO, 0, 0, 0,0,T1_INTERRUPT_NORMAL);
  timer2Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_64, 0, 0, 0,T0_POLLING);

  timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_256, 0, 0, T0_INTERRUPT_NORMAL); //T0_PRESCALER_64
  // timer1Init(T1_COMP_MODE_OCR1A_TOP,T1_OC1_DIS,T0_PRESCALER_256, 0, 0, 0,0,T1_INTERRUPT_NORMAL);
  //sei();
  while(1)
  {
	  
	  
  }	
}





ISR(TIMER0_OVF_vector)
{
	static volatile uint8_t flag =0;
	if(spwm_duty_cycle==0)
	{
		gpioPinWrite(MOTOR_EN_1_GPIO, spwm_pin_1, LOW);
		gpioPinWrite(MOTOR_EN_2_GPIO, spwm_pin_2, LOW);

	}else
	{
		if (spwm_duty_cycle==100)
		{
	      gpioPinWrite(MOTOR_EN_1_GPIO, spwm_pin_1, HIGH);
	      gpioPinWrite(MOTOR_EN_2_GPIO, spwm_pin_2, HIGH);
		}else
		{
			if(flag==0)
			{
			    gpioPinWrite(MOTOR_EN_1_GPIO, spwm_pin_1, HIGH);
		        gpioPinWrite(MOTOR_EN_2_GPIO, spwm_pin_2, HIGH);
				TCNT0=55+(200 - (spwm_duty_cycle*2));
				flag=1;
			}else
			{
			    gpioPinWrite(MOTOR_EN_1_GPIO, spwm_pin_1, LOW);
			    gpioPinWrite(MOTOR_EN_2_GPIO, spwm_pin_2, LOW);
				TCNT0=55+(spwm_duty_cycle*2);
				flag=0;
			}

			
		}
	}

}